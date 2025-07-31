#include <GL/glew.h>
#include <GL/freeglut.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl3.h"

#include "globals.h"
#include "config.h"
#include "tilemap.h"
#include "tileset.h"
#include "gui.h"

int main(int argc, char** argv) {
    LoadConfig("Games/Template Game/template-game.config");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Tile Editor");

    glewInit();
    tileset->load(config.tilesetFilename,config.tileSize);
    tilemap->load(config.mapFilename,config.tilemapAddress, config.mapRows, config.mapCols,config.topToBottom);

    Initialize();

    // Setup ImGui
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL3_Init("#version 330");

    glutDisplayFunc(Display);
    glutIdleFunc(glutPostRedisplay);

    glutMainLoop();
    return 0;
}
