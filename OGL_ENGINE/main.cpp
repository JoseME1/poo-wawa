
#include <GLFW/glfw3.h>
#include <engine/Billboard.h>
#include <engine/CollisionBox.h>
#include <engine/Objectives.h>
#include <engine/Particles.h>
#include <engine/Plane.h>
#include <engine/QuadTexture.h>
#include <engine/RigidModel.h>
#include <engine/Terrain.h>
#include <engine/functions.h>
#include <engine/shader_m.h>
#include <engine/skybox.h>
#include <engine/textrenderer.h>
#include <glad/glad.h>
#include <iostream>
#include<thread>

int i = 0;

int main()
{
    //:::: INICIALIZAMOS GLFW CON LA VERSIÓN 3.3 :::://
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    //:::: CREAMOS LA VENTANA:::://
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ejemplo OGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //:::: OBTENEMOS INFORMACIÓN DE TODOS LOS EVENTOS DE LA VENTANA:::://
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetJoystickCallback(joystick_callback);

    //:::: DESHABILITAMOS EL CURSOR VISUALMENTE EN LA PANTALLA :::://
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //:::: INICIALIZAMOS GLAD CON LAS CARACTERÍSTICAS DE OPENGL ESCENCIALES :::://
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    //INICIALIZAMOS LA ESCENA
    Shader ourShader("shaders/multiple_lighting.vs", "shaders/multiple_lighting.fs");
    Shader selectShader("shaders/selectedShader.vs", "shaders/lighting_maps.fs");

    initScene(ourShader);
    //Aqui se debe cambiar la ruta del mapa de alturas
    
    Terrain terrain("textures//heightmap3.jpg", texturePaths);

    SkyBox sky(2.0f, "7");

    /*Declaran el plano del agua y el billboard del arbol y el fuego*/
    Plane agua; 
    Billboard arbol;
    Billboard tornado;
    Billboard lluvia_fuego;
    Particles chispa;
    ParticleProps m_chispa;

    QuadTexture* corazon = new QuadTexture("textures//Vida//ratavida.png", 506, 461.0f, 0, 0);

    TextRenderer* municion = new TextRenderer(SCR_WIDTH, SCR_HEIGHT);

   

    cb = isCollBoxModel ? &models[indexCollBox].collbox : &collboxes.at(indexCollBox).second;

    //coso 
   lluvia_fuego = Billboard("textures/FireSprite.png", (float)SCR_WIDTH, (float)SCR_HEIGHT, 172.5, 210);

    agua = Plane("textures/water.png", 494.0, 740.0, 0, 0);

    arbol = Billboard("textures/FireSprite.png", (float)SCR_WIDTH, (float)SCR_HEIGHT, 172.5, 210);
    tornado = Billboard("textures/tornado.png", (float)SCR_WIDTH, (float)SCR_HEIGHT, 325, 621);



    TextRenderer* mensaje = new TextRenderer(SCR_WIDTH, SCR_HEIGHT);

 
    mensaje->Load("fonts/OpenSans-Light.TTF", 60);


    //:::: RENDER:::://

    while (!glfwWindowShouldClose(window))
    {
        //::::TIMING:::://Ayuda a crear animaciones fluidas
        float currentFrame = glfwGetTime();
        deltaTime = (currentFrame - lastFrame);
        lastFrame = currentFrame;
        respawnCount += 0.1;

        //::::ENTRADA CONTROL:::://
        processInput(window);
        //:::: LIMPIAMOS BUFFERS:::://
        glClearColor(0.933f, 0.811f, 0.647f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //:::: PASAMOS INFORMACIÓN AL SHADER:::://
        ourShader.use();

        //:::: DEFINICIÓN DE MATRICES::::// La multiplicaciónd e model*view*projection crea nuestro entorno 3D
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 300.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        //:::: RENDER DE MODELOS:::://
        drawModels(&ourShader, view, projection);
        //:::: SKYBOX Y TERRENO:::://
       
        loadEnviroment(&terrain, &sky, view, projection);
        
        if (salud > 0) {
           
            corazon->Draw(glm::vec2(-0.8f, 0.8f), 0.3);
        }
       
 
        if (salud > 100) {
          
            corazon->Draw(glm::vec2(-0.5f, 0.8f), 0.3);
          
        }
       
        //Agua--- Se inicializa el plano con las dimensiones de las texturas que se usaran, la posicion, el angulo en el que se rotara si es que se necesita
        // la escala del tamaño y se dibuja

        if (mov_agua_x<=4.0f && mov_agua_y <=4.0f && !waterout) {
            mov_agua_x += 0.002f;
            mov_agua_y += 0.002f;
        }
        else {
            waterout = true;
            if (mov_agua_x > 0.0f && mov_agua_y > 0.0f && waterout) {
                mov_agua_x -= 0.002f;
                mov_agua_y -= 0.002f;
            }
            else
                waterout = false;
        }

        if (direccion_1) {
            if (posicion_agua.x < 37)
            {
                posicion_agua.x += 0.002;
            }
            else {
                direccion_1 = false;

            }
        }
        else {

            if (posicion_agua.x > 35) {
                posicion_agua.x -= 0.002;
            }
            else
            {

                direccion_1 = true;
            }
        }


        //Lluvia
        lluvia_fuego.setScale(0.5f);
        
        agua.setPosition(glm::vec3(22.0f, -1.20, 20.0));
        agua.setAngles(glm::vec3(90.0f, 0.0f, 0.0f));
        agua.setScale(glm::vec3(15.0, 18.0, 16));

        agua.draw(mov_agua_x, mov_agua_y, view, projection);

        //billboard-- Se inicializa indicando la textura con transparencia que se usara, las dimensiones de la ventana y de la textura
        //Tambien se agrega una posicion con un vec3 una escala mas grande y de parametros se agrega la camara y como no tendra animacion
        //Se deja en ceros los otros 2 parametros
        contador++;
        if (fire_anim_Y <= 2.0f) {

            if (contador < 100000) {
                if (fire_anim_X <= 4.0f) {
                    fire_anim_X += 0.8f;
                }
                else
                {
                    //Descomentar Y para trabajar el recorrido hacia bajo del spritesheet
                    //fire_anim_Y += 1.0f;
                    fire_anim_X = 0.0f;
                }
                contador = 0;
            }
           
        }
        else {
            if (contador < 100000) {
            // Al llegar al final se reinicia la posicion del primer sprite
            //fire_anim_Y = 0.0f;
            fire_anim_X = 1.0f;
            contador = 0;
            }
        }

        arbol.setPosition(position_arbol1);
        arbol.setScale(6.0f);
        arbol.Draw(camera, round(fire_anim_X), round(fire_anim_Y));

        //Mas arboles solo cambiando la posicion
        arbol.setPosition(glm::vec3(10.0f,2.0f,10));
        arbol.Draw(camera, 0, 0);

        if (true) {
            for (int i = 0; i < 2000; i++) {

                lluvia_fuego.setPosition({ gotas_lluviaX[i], gotas_lluviaY[i], gotas_lluviaZ[i] });
                lluvia_fuego.Draw(camera, round(fire_anim_X), round(fire_anim_Y));

            }
        }

        //Animacion de Billboard
       
        //Proceso para sacar las posiciones aleatorias de las gotas de fuego
        int indice_lluvia = 0;

        //Solo se ejecutara la primera vez que inicia el programa
        //NOTA: LAS VARIABLES UTILIZADAS EN EL SIGUIENTE PROCESO FUERON DECLARADAS EN EL ARCHIVO Variables.h
        while (primera_vez) {

            //Mientras sea menor a la cantidad de elementos que se especifica se calcularan las posiciones en x,y,z utilizando la funcion rand
            if (indice_lluvia < 2000) {
                gotas_lluviaX[indice_lluvia] = 0.5 * (rand() % 1000);
                gotas_lluviaZ[indice_lluvia] = 0.5 * (rand() % 1000);

                //guarda la posicion inical en Y, para cuando a cero se reinicie la lluvia
                gotas_lluviaY_Inicial[indice_lluvia] = gotas_lluviaY[indice_lluvia] = 15 + rand() % 18;
                indice_lluvia++;
            }
            else
            {
                primera_vez = false;
            }

        }

        //Aqui se trabaja el descenso de la lluvia en Y
        for (int i = 0; i < 2000; i++) {
            //Mientras sea mayor igual a cero seguira la caida
            if (gotas_lluviaY[i] >= 0) {
                gotas_lluviaY[i] -= caida_lluvia;
            }
            else {
                //Cuando se menor a cero se inicializara de nuevo la caida con la posicion inical en Y
                gotas_lluviaY[i] = gotas_lluviaY_Inicial[i];
            }
        }

 
        //:::: COLISIONES :::://
        collisions();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //:::: LIBERACIÓN DE MEMORIA:::://   
    delete[] texturePaths;
   
    sky.Release();

    terrain.Release();


    delete municion;

    //agua.Release();
    for (int i = 0; i < lightcubes.size(); i++)
        lightcubes[i].second.Release();
    for (int i = 0; i < collboxes.size(); i++)
        collboxes[i].second.Release();
    for (int i = 0; i < models.size(); i++)
        models[i].Release();
    for (int i = 0; i < rigidModels.size(); i++)
    {
        physicsEnviroment.Unregister(rigidModels[i].getRigidbox());
    }
    physicsEnviroment.Unregister(&piso);
    physicsEnviroment.Unregister(&pared);
    glfwTerminate();

    return 0;
}

void initScene(Shader ourShader)
{
    camera.PosPersonaje = glm::vec3(23.0f, 2.75f, 29.0f);
    //Aqui se deben inicializar las posiciones random iniciales de los objetos y las posiciones fijas para utilizarlas despues
    //Inicializar posicion de billboard
    position_arbol1 = glm::vec3(30.0f, 10.0f, 30.0f);
    //:::: DEFINIMOS LAS TEXTURAS DE LA MULTITEXTURA DEL TERRENO :::://
    texturePaths = new const char *[4];
    texturePaths[0] = "textures/multitextura_3.png";
    texturePaths[1] = "textures/GreenGrass_Terrain01.png";
    texturePaths[2] = "textures/GreenGrass_Terrain02.png";
    texturePaths[3] = "textures/GreenGrass_Terrain03.jpg";

    //:::: POSICIONES DE TODAS LAS LUCES :::://
    pointLightPositions.push_back(glm::vec3(20.3f, 5.2f, 20.0f));
    pointLightPositions.push_back(glm::vec3(2.3f, 10.3f, -4.0f));
    pointLightPositions.push_back(glm::vec3(1.0f, 9.3f, -7.0f));
    pointLightPositions.push_back(glm::vec3(0.0f, 10.0f, -3.0f));

    //:::: POSICIONES DE TODOS LOS OBJETOS CON FISICAS :::://
    physicsObjectsPositions.push_back(glm::vec3(0.0, 10.0, 0.0));
    physicsObjectsPositions.push_back(glm::vec3(2.0, 10.0, 0.0));
    physicsObjectsPositions.push_back(glm::vec3(1.0, 10.0, 0.0));
    physicsObjectsPositions.push_back(glm::vec3(-2.0, 10.0, -2.0));
    physicsObjectsPositions.push_back(glm::vec3(-2.0, 10.0, -2.0));
    physicsObjectsPositions.push_back(glm::vec3(15.0, 1.0, -2.0));
    physicsObjectsPositions.push_back(glm::vec3(15.0, 1.0, -2.0));
    physicsObjectsPositions.push_back(glm::vec3(25.0, 10.0, -2.0));


    //:::: ESTADO GLOBAL DE OPENGL :::://
    glEnable(GL_DEPTH_TEST);

    //Definimos los collbox de la camara
    camera.setCollBox();

    //:::: CARGAMOS LOS SHADERS :::://
    ourShader.use();

    //:::: INICIALIZAMOS NUESTROS MODELOS :::://    
    //Nombre, posición, angulo, escala
    //initscale - escala ya definida - 0.2f
    models.push_back(Model("black-cat", "models/black-cat/black-cat.obj", glm::vec3(20.2, 2.9, -22.0), glm::vec3(0, 0, 0), 0.0f, 1.0f));
    models.push_back(Model("black-cat-02", "models/black-cat-02/black-cat-02.obj", glm::vec3(4.5, 6.7, -28.5), glm::vec3(25, 0, 0), 0.0f, 1.5f));
    models.push_back(Model("cat-03", "models/cat-03/cat-03.obj", glm::vec3(25.0, 2.9, -31.0), glm::vec3(0, 0, 0), 0.0f, 0.1f));
    //Quesos
    models.push_back(Model("cheese", "models/cheese/cheese.obj", glm::vec3(12.0, 3.0, -28.0), glm::vec3(0, 0, 0), 0.0f, 0.3f));
    models.push_back(Model("cheese", "models/cheese/cheese.obj", glm::vec3(30.0, 3.0, -38.0), glm::vec3(0, 0, 0), 0.0f, 0.3f));
    models.push_back(Model("cheese", "models/cheese/cheese.obj", glm::vec3(-3.0, 3.0, -30.0), glm::vec3(0, 0, 0), 0.0f, 0.3f));

    models.push_back(Model("female-owner", "models/female-owner/female-owner.obj", glm::vec3(12.0, 2.90, -35.0), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 1.0f));
    models.push_back(Model("skeleton", "models/skeleton/skeleton.obj", glm::vec3(-10.25, .5, -35.0), glm::vec3(0.0, -45.0, 0.0), 0.0f, 1.0f));
    models.push_back(Model("wizard-gnome", "models/wizard-gnome/wizard-gnome.obj", glm::vec3(20.0, 1.0, 23.5), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.8f));
    //ratas
    models.push_back(Model("rat", "models/rat/rat.obj", glm::vec3(4.0, 2.0, 32.5), glm::vec3(0, 270, 0), 0.0f, 0.2f));
    models.push_back(Model("rat-02", "models/rat-02/rat-02.obj", glm::vec3(10.0, 2.9, -14.8), glm::vec3(0, 0, 0), 0.0f, 1.0f));
    models.push_back(Model("rat-03", "models/rat-03/rat-03.obj", glm::vec3(13.5, 2.9, -14.8), glm::vec3(0, 180, 0), 0.0f, 0.15f));

    models.push_back(Model("pidgeon", "models/pidgeon/pidgeon.obj", glm::vec3(-7.0, 1.5, 14.2), glm::vec3(0, -50, 0), 0.0f, 0.20f));
    models.push_back(Model("car", "models/car/car.obj", glm::vec3(-10.0, 2.5, -22.0), glm::vec3(0, 90, 0), 0.0f, 1.0f));
    models.push_back(Model("dumpster", "models/dumpster/dumpster.obj", glm::vec3(2.0, 3.75, 34.5), glm::vec3(0, 180, 0), 0.0f, 2.0f));

    //arboles
    models.push_back(Model("tall-tree", "models/tall-tree/tall-tree.obj", glm::vec3(34.0, 1.0, -24.0), glm::vec3(0, 0, 0), 0.0f, 1.75f));
    models.push_back(Model("tree", "models/tree/tree.obj", glm::vec3(26.0, -1.10, -10.0), glm::vec3(0, 0, 0), 0.0f, 1.0f));
    models.push_back(Model("grass", "models/grass/grass.obj", glm::vec3(22.0, -1.0, -8.0), glm::vec3(0, 0, 0), 0.0f, 0.85));

    //Estructura cargada
    models.push_back(Model("haunted-house", "models/haunted-house/haunted-house-base.obj", glm::vec3(12, 1.75, -16.0), glm::vec3(0, 0, 0), 0.0f, 18.0f));
    models.push_back(Model("haunted-house", "models/haunted-house/haunted-house-cement.obj", glm::vec3(12, 1.75, -16.0), glm::vec3(0, 0, 0), 0.0f, 18.0f));
    models.push_back(Model("haunted-house", "models/haunted-house/haunted-house-bed-sofa.obj", glm::vec3(12, 1.75, -16.0), glm::vec3(0, 0, 0), 0.0f, 18.0f));
    models.push_back(Model("haunted-house", "models/haunted-house/haunted-house-ceramic.obj", glm::vec3(12, 1.75, -16.0), glm::vec3(0, 0, 0), 0.0f, 18.0f));
    models.push_back(Model("haunted-house", "models/haunted-house/haunted-house-furniture.obj", glm::vec3(12, 1.75, -16.0), glm::vec3(0, 0, 0), 0.0f, 18.0f));

   
  /*  models.push_back(Model("GarageBuilding", "models/GarageBuilding/GarageBuilding.obj", glm::vec3(12, 0.8, -4.5), glm::vec3(0, 90, 0), 0.0f, 1.0f));
   */ 
    //CREAMOS TODAS  LAS CAJAS DE COLISION INDIVIDUALES
    CollisionBox collbox;

    glm::vec4 colorCollbox(0.41f, 0.2f, 0.737f, 0.00f);
    collbox = CollisionBox(position_arbol1, glm::vec3(3.0f), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(0, pair<string, CollisionBox>("arbol1", collbox)));
    collbox = CollisionBox(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(5, 5, 5), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(1, pair<string, CollisionBox>("switch_luz", collbox)));
   collbox = CollisionBox(glm::vec3(23.0f, 1.5f, 20.0f), glm::vec3(2.0f,4.5f,2.0f), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(2, pair<string, CollisionBox>("obstaculo_salida", collbox)));
    collbox = CollisionBox(glm::vec3(posicion_trampa), glm::vec3(2.0f, 4.5f, 2.0f), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(3, pair<string, CollisionBox>("trampa", collbox)));
    //Si queremos agregar más copiar y pegar las 2 líneas de abajo
    //2 parametros caja de colision-nombre que va a tener
    collbox = CollisionBox(glm::vec3(posicion_trampa), glm::vec3(2.0f, 4.5f, 2.0f), colorCollbox);
    collboxes.insert(pair<int, pair<string, CollisionBox>>(4, pair<string, CollisionBox>("tormenta", collbox)));
    //Arreglo de colisiones de Enemigos
    

    //CREAMOS LOS LIGHTCUBES QUE ENREALIDAD SON COLLISION BOXES QUE NOS AYUDARAN A IDENTIFICAR LA POSICIÓN DE DONDE ESTAN
    glm::vec3 lScale(0.5);
    colorCollbox = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    collbox = CollisionBox(pointLightPositions[0], lScale, colorCollbox);
    lightcubes.insert(pair<int, pair<string, CollisionBox>>(0, pair<string, CollisionBox>("LUZ1", collbox)));
    collbox = CollisionBox(pointLightPositions[1], lScale, colorCollbox);
    lightcubes.insert(pair<int, pair<string, CollisionBox>>(1, pair<string, CollisionBox>("LUZ2", collbox)));
    collbox = CollisionBox(pointLightPositions[2], lScale, colorCollbox);
    lightcubes.insert(pair<int, pair<string, CollisionBox>>(2, pair<string, CollisionBox>("LUZ3", collbox)));
    collbox = CollisionBox(pointLightPositions[3], lScale, colorCollbox);
    lightcubes.insert(pair<int, pair<string, CollisionBox>>(3, pair<string, CollisionBox>("LUZ4", collbox)));

   
     
}
//:::: CONFIGURACIONES :::://

void loadEnviroment(Terrain *terrain, SkyBox *sky, glm::mat4 view, glm::mat4 projection)
{
    glm::mat4 model = mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0, -2.5f, 0.0f));   // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(100.5f, 100.0f, 100.5f)); // it's a bit too big for our scene, so scale it down
    //model = glm::rotate(model, glm::radians(rodandoYmenos--), glm::vec3(0, 1, 0));

    terrain->draw(model, view, projection);
    terrain->getShader()->setFloat("shininess", 10.0f);

    setMultipleLight(terrain->getShader(), pointLightPositions);

   
    glm::mat4 skyModel = mat4(1.0f);
    skyModel = glm::translate(skyModel, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    skyModel = glm::scale(skyModel, glm::vec3(60.0f, 60.0f, 60.0f));  // it's a bit too big for our scene, so scale it down
    //skyModel = glm::rotate(skyModel, glm::radians(rodandoY++), glm::vec3(0, 1, 0));

    sky->draw(skyModel, view, projection, skyPos);
    sky->getShader()->setFloat("shininess", 10.0f);
    setMultipleLight(sky->getShader(), pointLightPositions);


    //RENDER DE LIGHT CUBES
    if (renderLightingCubes)
        for (pair<int, pair<string, CollisionBox>> lights : lightcubes)
            lights.second.second.draw(view, projection);

}
void drawModels(Shader *shader, glm::mat4 view, glm::mat4 projection)
{

  
    //DEFINIMOS EL BRILLO  DEL MATERIAL
    shader->setFloat("material.shininess", 10.0f);
    setMultipleLight(shader, pointLightPositions);   
  
    camera.Position.y = 3;

    for (int i = 0; i < models.size(); i++)
    {
       //SI SE RECOGIO EL OBJETO
        if (!models[i].isActive())
            continue;

        shader->use();

        if (models[i].name == "cheese") {

           models[i].setAngles(glm::vec3(0, giro+=.500, 0));

           //madre pa "eliminar"
           if (intersect(models[i].collbox, camera.collbox)) {
               models[i].setActive(false);
               continue;
           }

        }

        if (models[i].name == "wizard-gnome") {

            models[i].setAngles(glm::vec3(0, giro += .500, 0));
        }

       models[i].Draw(*shader);

     
        //Detecta las colisiones entre objetos cargados con el importador
        detectColls(&models[i].collbox, models[i].name, &camera, renderCollBox, collidedObject_callback);


    }

    
    
}

void setSimpleLight(Shader *shader)
{
    shader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    shader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    shader->setInt("lightType", (int)lightType);
    shader->setVec3("light.position", lightPos);
    shader->setVec3("light.direction", lightDir);
    shader->setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
    shader->setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
    shader->setVec3("viewPos", camera.Position);
    shader->setFloat("light.constant", 0.2f);
    shader->setFloat("light.linear", 0.02f);
    shader->setFloat("light.quadratic", 0.032f);
    shader->setFloat("material.shininess", 60.0f);
}
void setMultipleLight(Shader *shader, vector<glm::vec3> pointLightPositions)
{
    shader->setVec3("viewPos", camera.Position);

    shader->setVec3("dirLights[0].direction", pointLightPositions[0]);
    shader->setVec3("dirLights[0].ambient", mainLight.x, mainLight.y, mainLight.z);
    shader->setVec3("dirLights[0].diffuse", mainLight.x, mainLight.y, mainLight.z);
    shader->setVec3("dirLights[0].specular", mainLight.x, mainLight.y, mainLight.z);

    shader->setVec3("dirLights[1].direction", pointLightPositions[1]);
    shader->setVec3("dirLights[1].ambient", 0.09f, 0.05f, 0.05f);
    shader->setVec3("dirLights[1].diffuse", 0.9f, 0.4f, 0.4f);
    shader->setVec3("dirLights[1].specular", 0.9f, 0.5f, 0.5f);

    shader->setVec3("dirLights[2].direction", pointLightPositions[2]);
    shader->setVec3("dirLights[2].ambient", 0.3f, 0.5f, 0.5f);
    shader->setVec3("dirLights[2].diffuse", 0.4f, 0.4f, 0.4f);
    shader->setVec3("dirLights[2].specular", 0.5f, 0.5f, 0.5f);

    shader->setVec3("dirLights[3].direction", pointLightPositions[3]);
    shader->setVec3("dirLights[3].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("dirLights[3].diffuse", 0.4f, 0.4f, 0.4f);
    shader->setVec3("dirLights[3].specular", 0.5f, 0.5f, 0.5f);

    shader->setVec3("pointLights[0].position", pointLightPositions[0]);
    shader->setVec3("pointLights[0].ambient", 1.0f, 1.0f, 1.0f);
    shader->setVec3("pointLights[0].diffuse", 1.0f, 1.0f, 1.0f);
    shader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[0].constant", 1.0f);
    shader->setFloat("pointLights[0].linear", 0.09);
    shader->setFloat("pointLights[0].quadratic", 0.032);

    shader->setVec3("pointLights[1].position", pointLightPositions[1]);
    shader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[1].constant", 1.0f);
    shader->setFloat("pointLights[1].linear", 0.09);
    shader->setFloat("pointLights[1].quadratic", 0.032);

    shader->setVec3("pointLights[2].position", pointLightPositions[2]);
    shader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[2].constant", 1.0f);
    shader->setFloat("pointLights[2].linear", 0.09);
    shader->setFloat("pointLights[2].quadratic", 0.032);

    shader->setVec3("pointLights[3].position", pointLightPositions[3]);
    shader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[3].constant", 1.0f);
    shader->setFloat("pointLights[3].linear", 0.09);
    shader->setFloat("pointLights[3].quadratic", 0.032);

    shader->setVec3("spotLights[0].position", pointLightPositions[0]);
    shader->setVec3("spotLights[0].direction", glm::vec3(0.2, 0.8, 0.2));
    shader->setVec3("spotLights[0].ambient", 1.0f, 1.0f, 1.0f);
    shader->setVec3("spotLights[0].diffuse", 1.0f, 1.0f, 1.0f);
    shader->setVec3("spotLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("spotLights[0].constant", 1.0f);
    shader->setFloat("spotLights[0].linear", 0.09);
    shader->setFloat("spotLights[0].quadratic", 0.032);
    shader->setFloat("spotLights[0].cutOff", glm::cos(glm::radians(12.5f)));
    shader->setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(15.0f)));

    // spotLight
    shader->setVec3("spotLights[1].position", pointLightPositions[1]);
    shader->setVec3("spotLights[1].direction", camera.Front);
    shader->setVec3("spotLights[1].ambient", 0.0f, 0.0f, 0.0f);
    shader->setVec3("spotLights[1].diffuse", 1.0f, 1.0f, 1.0f);
    shader->setVec3("spotLights[1].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("spotLights[1].constant", 1.0f);
    shader->setFloat("spotLights[1].linear", 0.09);
    shader->setFloat("spotLights[1].quadratic", 0.032);
    shader->setFloat("spotLights[1].cutOff", glm::cos(glm::radians(12.5f)));
    shader->setFloat("spotLights[1].outerCutOff", glm::cos(glm::radians(15.0f)));

    shader->setVec3("spotLights[2].position", pointLightPositions[2]);
    shader->setVec3("spotLights[2].direction", camera.Front);
    shader->setVec3("spotLights[2].ambient", 0.0f, 0.0f, 0.0f);
    shader->setVec3("spotLights[2].diffuse", 1.0f, 1.0f, 1.0f);
    shader->setVec3("spotLights[2].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("spotLights[2].constant", 1.0f);
    shader->setFloat("spotLights[2].linear", 0.09);
    shader->setFloat("spotLights[2].quadratic", 0.032);
    shader->setFloat("spotLights[2].cutOff", glm::cos(glm::radians(12.5f)));
    shader->setFloat("spotLights[2].outerCutOff", glm::cos(glm::radians(15.0f)));

    shader->setVec3("spotLights[3].position", pointLightPositions[3]);
    shader->setVec3("spotLights[3].direction", camera.Front);
    shader->setVec3("spotLights[3].ambient", 0.0f, 0.0f, 0.0f);
    shader->setVec3("spotLights[3].diffuse", 1.0f, 1.0f, 1.0f);
    shader->setVec3("spotLights[3].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("spotLights[3].constant", 1.0f);
    shader->setFloat("spotLights[3].linear", 0.09);
    shader->setFloat("spotLights[3].quadratic", 0.032);
    shader->setFloat("spotLights[3].cutOff", glm::cos(glm::radians(12.5f)));
    shader->setFloat("spotLights[3].outerCutOff", glm::cos(glm::radians(15.0f)));

    shader->setInt("lightType", (int)lightType);
    shader->setInt("maxRenderLights", 1);
}


void collisions()
{
    //Detecta las colisiones de las cajas individuales
    //TODO LO DE LAS COLISIONES VA AQUÍ
    detectColls(collboxes, &camera, renderCollBox, collidedObject_callback);
}