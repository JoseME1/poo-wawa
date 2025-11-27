
#include <GLFW/glfw3.h>
#include <engine/Billboard.h>
#include <engine/CollisionBox.h>
#include <engine/Objectives.h>
#include <engine/Particles.h>
#include <engine/Plane.h>
#include <engine/QuadTexture.h>
#include <engine/RigidModel.h>
#include <engine/Terrain.h>
#include <engine/shader_m.h>
#include <engine/skybox.h>
#include <engine/textrenderer.h>
#include <engine/Audio.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>

enum LightType
{
    FlatColor,
    AllLights,
    DirectionalLight,
    SpotLight,
    PointLight
};

enum Axis
{
    X,
    Y,
    Z
};
enum TypeActionKeyBoard
{
    GAME,
    OBJECTS,
    COLLISION,
    LIGHTING,
};

enum TransformObject
{
    MODEL,
    COLLBOX,
};

//:::: ENUMS :::://
LightType lightType = FlatColor;
TypeActionKeyBoard typemenu = GAME;
TransformObject transformObject = COLLBOX;

//:::: VARIABLES GLOBALES :::://

//:::: BANDERAS :::://
// 
//VENTANA
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//CONTROL
double xJoy = 0.0;
double yJoy = 0.0;
double yLeftJoy = 0.0;
bool isJoyStick = false;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//TIMING
float deltaTime = 0.0f;
float lastFrame = 0.0f;
//TEXTURAS DEL TERRENO
const char **texturePaths;

//PARTICULAS
float respawnCount = 0.0f;

//:::: OBJETOS :::://
Camera camera(glm::vec3(0.0f, 20.0f, 30.0f));
float posicion_y = 3.0f;
float contador_caida = 0.0f;
float posicion_suelo = 3.0f;
float maxima_altura = 5.0f;
float retroceso = 5.0f;
float giro = 90.0f;
float girox = 90.0f;
float giroz = 90.0f;
float trayecto_retroceso = 5.0f;

bool saltar = false;
bool daño_caida = false;
bool trampa_move = false;
//Audio
Audio efecto1;
//movimiento
bool direccion_1 = true;
bool direccion_2 = false;
bool waterout = false;
//
bool control_cielo=true;

glm::vec3 posicion_agua = glm::vec3(35.0f, 0.5f, 35.0f);
glm::vec3 posicion_2 = glm::vec3(-35.0f, 0.5f, -35.0f);


glm::vec3 posicion_torreta = glm::vec3(30.0f,3.0f, 10.0f);
glm::vec3 posicion_bala = glm::vec3(30.0f, 3.0f, 20.0f);
float posicion_trayecto = 0.0f;
float posicion_final_trayecto = 10.0f;

float mov_agua_x = 0.0f;
float mov_agua_y = 0.0f;

glm::vec3 posicion_trampa = glm::vec3(30.0f, 1.5f, 20.0f);
glm::vec3 posicion_anakin = glm::vec3(0.0, 2.5f, 5.0f);
bool crear_bala = false;
//Variables Juego
bool choco = false;
bool choco_torre = false;

bool sonar_efecto1 = false;
float salud = 101.0f;
bool control_salud = true;
bool golpe = true;
int limite_choque = 0;
int contador = 0;
//Variable utilizada para definir la posicion del billboard y a su caja de colision
glm::vec3 position_arbol1;

//

//Variables utilizadas para la animacion del billboard
float fire_anim_Y = 0.0f;
float fire_anim_X = 0.0f;


//Variables utilizadas para la animacion del billboard
float tornado_anim_Y = 0.0f;
float tornado_anim_X = 0.0f;


float rodandoY = 0.0f;
float rodandoYmenos = 0.0f;

//Variables utilizadas para la dispercion de la lluvia
float posicion_lluvia = 10.0f;
float caida_lluvia = 0.5;
float gotas_lluviaX[2000]; //guarda las posiciones
float gotas_lluviaZ[2000];
float gotas_lluviaY[2000];
float gotas_lluviaY_Inicial[2000]; //posición inicial de las 2000 gotas
bool primera_vez = true; //booleana para que se genere por primera vez el posicionamiento de todas las gotas

//:::: VECTORES :::://
glm::vec3 force(0);
glm::vec3 posModel(0);
glm::vec3 rotationModel(0);
glm::vec3 scaleCollBox(0);
glm::vec3 skyPos(0);
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightDir(0.0f, 1.0f, 0.0f);
vec3 originPoint(0.0f, 1.0f, 0.0f);
vec3 rotationPoint(0.0, 1.0, 0.0);
vec3 mainLight(0.4);

//::::::::::::::MOVER OBJETOS:::::::::::::://
float initScale = 0.2f;
int indexObject = 0;
int indexCollBox = 0;
int indexLight = 0;

bool isCollBoxModel = false;
bool renderCollBox = false;
bool renderLightingCubes = false;
float angleObjects = 0.0f;
float movement = 0.01;

//Enemigos
bool destruir_enemigo_1 = false;

//::::::::::::::ARREGLOS DE OBJETOS:::::::::::::://
vector<glm::vec3> pointLightPositions;
vector<glm::vec3> physicsObjectsPositions;

vector<RigidModel> rigidModels;
vector<RigidModel> rbmodels = rigidModels;

vector<Model> models;

map<int, pair<string, CollisionBox>> collboxes;
map<int, pair<string, CollisionBox>> collboxes_enemigos;
map<int, pair<string, CollisionBox>> lightcubes;
CollisionBox* cb = new CollisionBox();
//::::::::::::::FISICAS:::::::::::::://
rbEnvironment physicsEnviroment;
rbRigidBody piso, pared;
int indexRigidModel = 0;
float colDetection = 0;

