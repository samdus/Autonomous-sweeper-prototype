///*
// * This file is part of the OpenKinect Project. http://www.openkinect.org
// *
// * Copyright (c) 2010 individual OpenKinect contributors. See the CONTRIB file
// * for details.
// *
// * This code is licensed to you under the terms of the Apache License, version
// * 2.0, or, at your option, the terms of the GNU General Public License,
// * version 2.0. See the APACHE20 and GPL2 files for the text of the licenses,
// * or the following URLs:
// * http://www.apache.org/licenses/LICENSE-2.0
// * http://www.gnu.org/licenses/gpl-2.0.txt
// *
// * If you redistribute this file in source form, modified or unmodified, you
// * may:
// *   1) Leave this header intact and distribute it under the same terms,
// *      accompanying it with the APACHE20 and GPL20 files, or
// *   2) Delete the Apache 2.0 clause and accompany it with the GPL2 file, or
// *   3) Delete the GPL v2 clause and accompany it with the APACHE20 file
// * In all cases you must keep the copyright notice intact and include a copy
// * of the CONTRIB file.
// *
// * Binary distributions must follow the binary distribution requirements of
// * either License.
// */
//
//#include <cstdlib>
//#include <iostream>
//#include <vector>
//#include "SceneCamera.h"
//#include "Decodeur.h"
//#include "Vectors.h"
//#include "CloudPointContainer.h"
//#include "Config.h"
//
//#if defined(__APPLE__)
//#include <GLUT/glut.h>
//#else
//#include <GL/glut.h>
//#endif
//
//Freenect::Freenect freenect;            // Freenect adapteur
//int window(0);                          // Glut window identifier
//int mx = -1, my = -1;                   // Previous mouse coordinates
//bool color = true;                      // Flag to indicate to use of color in the cloud
//bool CarteVisible = true;               // Flag to show the map or not
//bool RobotVisible = true;               // Flag to show the map or not
//SceneCamera ViewCam = SceneCamera();    // Camera to navigate inside the generated map
//Decodeur DecodeurScene = Decodeur();    // Decodeur de la scene (intelligence du robot)
//Vector3 robot[47];                      // Modele du robot
//Vector3 robotMesh[23];                  // Mesh du robot
//Vector3 kinectFOV[23];                  // Field of view de la kinect
//
//void InitRobotMesh()
//{
//    float w = std::stof(Config::Instance().GetString("LARGEUR_ROBOT")) / 2;
//    float l = std::stof(Config::Instance().GetString("LONGEUR_ROBOT")) / 2;
//    float h = std::stof(Config::Instance().GetString("HAUTEUR_ROBOT"));
//    float kh = std::stof(Config::Instance().GetString("HAUTEUR_KINECT"));
//    float ko = std::stof(Config::Instance().GetString("OFFSET_KINECT"));
//    float fv = std::stof(Config::Instance().GetString("VERTICAL_FOV"));
//    float fh = std::stof(Config::Instance().GetString("HORIZONTAL_FOV"));
//    float nc = std::stof(Config::Instance().GetString("NEAR_CLIPPING"));
//    float fc = std::stof(Config::Instance().GetString("FAR_CLIPPING"));
//
////bottom
//    robot[0]  = Vector3(w,0.0,l);
//    robot[1]  = Vector3(w,0.0,-l);
//    robot[2]  = Vector3(-w,0.0,l);
//    robot[3]  = Vector3(w,0.0,-l);
//    robot[4]  = Vector3(-w,0.0,l);
//    robot[5]  = Vector3(-w,0.0,-l);
//
////left
//    robot[6]  = Vector3(-w,0.0,l);
//    robot[7]  = Vector3(-w,0.0,-l);
//    robot[8]  = Vector3(-w,h,l);
//    robot[9]  = Vector3(-w,0.0,-l);
//    robot[10] = Vector3(-w,h,l);
//    robot[11] = Vector3(-w,h,-l);
//
////front
//    robot[12] = Vector3(-w,0.0,-l);
//    robot[13] = Vector3(w,0.0,-l);
//    robot[14] = Vector3(-w,h,-l);
//    robot[15] = Vector3(w,0.0,-l);
//    robot[16] = Vector3(-w,h,-l);
//    robot[17] = Vector3(w,h,-l);
//
////rigth
//    robot[18] = Vector3(w,0.0,l);
//    robot[19] = Vector3(w,0.0,-l);
//    robot[20] = Vector3(w,h,l);
//    robot[21] = Vector3(w,0.0,-l);
//    robot[22] = Vector3(w,h,l);
//    robot[23] = Vector3(w,h,-l);
//
////back
//    robot[24] = Vector3(-w,0.0,l);
//    robot[25] = Vector3(w,0.0,l);
//    robot[26] = Vector3(-w,h,l);
//    robot[27] = Vector3(w,0.0,l);
//    robot[28] = Vector3(-w,h,l);
//    robot[29] = Vector3(w,h,l);
//
////top
//    robot[30] = Vector3(w,h,l);
//    robot[31] = Vector3(w,h,-l);
//    robot[32] = Vector3(-w,h,l);
//    robot[33] = Vector3(w,h,-l);
//    robot[34] = Vector3(-w,h,l);
//    robot[35] = Vector3(-w,h,-l);
//
////camtop
//    robot[36] = Vector3(0.0,kh,-ko);
//    robot[37] = Vector3(w+50.0,kh+50,-l-50.0);
//    robot[38] = Vector3(-w-50.0,kh+50,-l-50.0);
////cambottom
//    robot[39] = Vector3(0.0,kh,-ko);
//    robot[40] = Vector3(w+50.0,kh-50,-l-50.0);
//    robot[41] = Vector3(-w-50.0,kh-50,-l-50.0);
////camleft
//    robot[42] = Vector3(0.0,kh,-ko);
//    robot[43] = Vector3(w+50.0,kh+50,-l-50.0);
//    robot[44] = Vector3(w+50.0,kh-50,-l-50.0);
////camrigth
//    robot[45] = Vector3(0.0,kh,-ko);
//    robot[46] = Vector3(-w-50.0,kh+50,-l-50.0);
//    robot[47] = Vector3(-w-50.0,kh-50,-l-50.0);
//
////robotMesh
//    robotMesh[0]  = Vector3(w,0.0,l);
//    robotMesh[1]  = Vector3(w,0.0,-l);
//    robotMesh[2]  = Vector3(w,0.0,-l);
//    robotMesh[3]  = Vector3(-w,0.0,-l);
//    robotMesh[4]  = Vector3(-w,0.0,-l);
//    robotMesh[5]  = Vector3(-w,0.0,l);
//    robotMesh[6]  = Vector3(-w,0.0,l);
//    robotMesh[7]  = Vector3(w,0.0,l);
//    robotMesh[8]  = Vector3(w,0.0,l);
//    robotMesh[9]  = Vector3(w,h,l);
//    robotMesh[10] = Vector3(-w,0.0,l);
//    robotMesh[11] = Vector3(-w,h,l);
//    robotMesh[12] = Vector3(w,0.0,-l);
//    robotMesh[13] = Vector3(w,h,-l);
//    robotMesh[14] = Vector3(-w,0.0,-l);
//    robotMesh[15] = Vector3(-w,h,-l);
//    robotMesh[16] = Vector3(w,h,l);
//    robotMesh[17] = Vector3(w,h,-l);
//    robotMesh[18] = Vector3(w,h,-l);
//    robotMesh[19] = Vector3(-w,h,-l);
//    robotMesh[20] = Vector3(-w,h,-l);
//    robotMesh[21] = Vector3(-w,h,l);
//    robotMesh[22] = Vector3(-w,h,l);
//    robotMesh[23] = Vector3(w,h,l);
//
////kinect fov
//    kinectFOV[0]  = Vector3(0.0,0.0 + kh,0.0 - ko);
//    kinectFOV[1]  = Vector3(-sin(fh/2 * DEG2RAD) * fc,-sin(fv/2 * DEG2RAD) * fc + kh,-fc - ko);
//    kinectFOV[2]  = Vector3(0.0,0.0 + kh,0.0 - ko);
//    kinectFOV[3]  = Vector3(-sin(fh/2 * DEG2RAD) * fc,sin(fv/2 * DEG2RAD) * fc + kh,-fc - ko);
//    kinectFOV[4]  = Vector3(0.0,0.0 + kh,0.0 - ko);
//    kinectFOV[5]  = Vector3(sin(fh/2 * DEG2RAD) * fc,sin(fv/2 * DEG2RAD) * fc + kh,-fc - ko);
//    kinectFOV[6]  = Vector3(0.0,0.0 + kh,0.0 - ko);
//    kinectFOV[7]  = Vector3(sin(fh/2 * DEG2RAD) * fc,-sin(fv/2 * DEG2RAD) * fc + kh,-fc - ko);
//    kinectFOV[8]  = Vector3(-sin(fh/2 * DEG2RAD) * fc,-sin(fv/2 * DEG2RAD) * fc + kh,-fc - ko);
//    kinectFOV[9]  = Vector3(-sin(fh/2 * DEG2RAD) * fc,sin(fv/2 * DEG2RAD) * fc + kh,-fc - ko);
//    kinectFOV[10] = Vector3(-sin(fh/2 * DEG2RAD) * fc,sin(fv/2 * DEG2RAD) * fc + kh,-fc - ko);
//    kinectFOV[11] = Vector3(sin(fh/2 * DEG2RAD) * fc,sin(fv/2 * DEG2RAD) * fc + kh,-fc - ko);
//    kinectFOV[12] = Vector3(sin(fh/2 * DEG2RAD) * fc,sin(fv/2 * DEG2RAD) * fc + kh,-fc - ko);
//    kinectFOV[13] = Vector3(sin(fh/2 * DEG2RAD) * fc,-sin(fv/2 * DEG2RAD) * fc + kh,-fc - ko);
//    kinectFOV[14] = Vector3(sin(fh/2 * DEG2RAD) * fc,-sin(fv/2 * DEG2RAD) * fc + kh,-fc - ko);
//    kinectFOV[15] = Vector3(-sin(fh/2 * DEG2RAD) * fc,-sin(fv/2 * DEG2RAD) * fc + kh,-fc - ko);
//    kinectFOV[16] = Vector3(-sin(fh/2 * DEG2RAD) * nc,-sin(fv/2 * DEG2RAD) * nc + kh,-nc - ko);
//    kinectFOV[17] = Vector3(-sin(fh/2 * DEG2RAD) * nc,sin(fv/2 * DEG2RAD) * nc + kh,-nc - ko);
//    kinectFOV[18] = Vector3(-sin(fh/2 * DEG2RAD) * nc,sin(fv/2 * DEG2RAD) * nc + kh,-nc - ko);
//    kinectFOV[19] = Vector3(sin(fh/2 * DEG2RAD) * nc,sin(fv/2 * DEG2RAD) * nc + kh,-nc - ko);
//    kinectFOV[20] = Vector3(sin(fh/2 * DEG2RAD) * nc,sin(fv/2 * DEG2RAD) * nc + kh,-nc - ko);
//    kinectFOV[21] = Vector3(sin(fh/2 * DEG2RAD) * nc,-sin(fv/2 * DEG2RAD) * nc + kh,-nc - ko);
//    kinectFOV[22] = Vector3(sin(fh/2 * DEG2RAD) * nc,-sin(fv/2 * DEG2RAD) * nc + kh,-nc - ko);
//    kinectFOV[23] = Vector3(-sin(fh/2 * DEG2RAD) * nc,-sin(fv/2 * DEG2RAD) * nc + kh,-nc - ko);
//}
//
//void DrawGLScene()
//{
//    InitRobotMesh();
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    std::vector<uint8_t> currentRgb;
//    std::vector<Vector3> currentDepth;
//    std::vector<uint16_t> realTimeDepth;
//
//    /*glPointSize(2.0f);
//
//    glBegin(GL_POINTS);
//
//
//    for(int k = 0; k < CLOUD_POINT_CIRCULAR_BUFFER; ++k)
//    {
//        currentRgb = DecodeurScene.cloudBuffer.GetCloudPointColor(k);
//        currentDepth = DecodeurScene.cloudBuffer.GetCloudPointDepth(k);
//
//        if (!color) glColor3ub(255, 255, 255);
//
//        if(!currentRgb.empty() && !currentDepth.empty())
//        {
//            for (int i = 0; i < currentDepth.size(); ++i)
//            {
//                glVertex3f(currentDepth[i].x, currentDepth[i].y, currentDepth[i].z);
//            }
//        }
//    }
//
//    glEnd();*/
//    //CLUSTER
//    glPointSize(3.0f);
//    glBegin(GL_POINTS);
//    for(int k = 0; k < DecodeurScene.convertisseur.Clusters.size(); ++k)
//    {
//        glColor3ub(125 + k*k*k*k/255, k * 255 / DecodeurScene.convertisseur.Clusters.size(),k);
//        currentDepth = DecodeurScene.convertisseur.Clusters[k];
//
//        if(!currentDepth.empty())
//        {
//            for (int i = 0; i < currentDepth.size(); ++i)
//            {
//                glVertex3f(currentDepth[i].x, 0, currentDepth[i].z);
//            }
//        }
//    }
//    glEnd();
//    glPointSize(2.0f);
//    glBegin(GL_POINTS);
//
//    //Real time cam
//    currentRgb = DecodeurScene.rgb;
//    realTimeDepth = DecodeurScene.depth;
//
//    if (!color) glColor3ub(255, 255, 255);
//
//    if(!currentRgb.empty() && !realTimeDepth.empty())
//    {
//        float HauteurMax = std::stof(Config::Instance().GetString("HAUTEUR_MAX"));
//        float HauteurMin = std::stof(Config::Instance().GetString("HAUTEUR_MIN"));
//        float HauteurKin = std::stof(Config::Instance().GetString("HAUTEUR_KINECT"));
//        float f = 595.f;
//        for (int i = 0; i < realTimeDepth.size(); ++i)
//        {
//            Vector3 vec = Vector3((i%IR_CAMERA_RESOLUTION_X - (IR_CAMERA_RESOLUTION_X-1)/2.f) * realTimeDepth[i] / f,
//                                  (-(i/IR_CAMERA_RESOLUTION_X - (IR_CAMERA_RESOLUTION_Y-1)/2.f) * realTimeDepth[i] / f) + HauteurKin,
//                                  -realTimeDepth[i]);
//
//            if(vec.y > HauteurMax || vec.y < HauteurMin )
//            {
//                continue;
//            }
//            if (color)
//                glColor3ub( currentRgb[3*i+0],    // R
//                            currentRgb[3*i+1],    // G
//                            currentRgb[3*i+2]);   // B
//
//            vec = DecodeurScene.RealCam.matrixToWorld * vec;
//            glVertex3f(vec.x, vec.y, vec.z);
//        }
//    }
//
//
//    glEnd();
//
//    // Draw the world coordinate frame
//    glLineWidth(2.0f);
//    glBegin(GL_LINES);
//    glColor3ub(255, 0, 0);  // Red   X-axis
//    glVertex3f(  0, 0, 0);
//    glVertex3f( 50, 0, 0);
//    glColor3ub(0, 255, 0);  // Green Y-axis
//    glVertex3f(0,   0, 0);
//    glVertex3f(0,  50, 0);
//    glColor3ub(0, 0, 255);  // Blue  Z-axis
//    glVertex3f(0, 0,   0);
//    glVertex3f(0, 0,  50);
//    glEnd();
//
//    if(CarteVisible) //Draw a plane
//    {
//        glLineWidth(5.0f);
//        glBegin(GL_LINES);
//        glColor3ub(255, 0, 0);  // Red   X-axis
//
//        std::vector<segment> segments = DecodeurScene.convertisseur.Environnement.GetSegments();
//        for(int i = 0; i < segments.size(); ++i)
//        {
//            glVertex3f(segments[i].debut.x, 0, segments[i].debut.y);
//            glVertex3f(segments[i].fin.x, 0, segments[i].fin.y);
//        }
//
//        glEnd();
//    }
//
//    if(RobotVisible) //Draw ROBOT and Kinect
//    {
//        glBegin(GL_TRIANGLES);
//            glColor4f(0.75, 0.75, 0.75, 0.5);
//            for(int i = 0; i < 36; ++i)
//            {
//                Vector3 vec = DecodeurScene.RealCam.matrixToWorld * robot[i];
//                glVertex3f(vec.x, vec.y, vec.z);
//            }
//            glEnd();
//
//            glLineWidth(3.0f);
//            glBegin(GL_LINES);
//            glColor3ub(0, 0, 0);
//            for(int i = 0; i < 24; ++i)
//            {
//                Vector3 vec = DecodeurScene.RealCam.matrixToWorld * robotMesh[i];
//                glVertex3f(vec.x, vec.y, vec.z);
//            }
//        glEnd();
//
//        glBegin(GL_TRIANGLES);
//            glColor4f(0.75, 0.75, 0.0, 0.5);
//            for(int i = 36; i < 48; ++i)
//            {
//                Vector3 vec = DecodeurScene.RealCam.matrixToWorld * robot[i];
//                glVertex3f(vec.x, vec.y, vec.z);
//            }
//            glEnd();
//
//            glLineWidth(1.0f);
//            glBegin(GL_LINES);
//            glColor3ub(0, 0, 0);
//            for(int i = 0; i < 24; ++i)
//            {
//                Vector3 vec = DecodeurScene.RealCam.matrixToWorld * kinectFOV[i];
//                glVertex3f(vec.x, vec.y, vec.z);
//            }
//        glEnd();
//    }
//
//    // Place the camera
//    // glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    glScalef(ViewCam.zoom, ViewCam.zoom, 1);
//    gluLookAt( ViewCam.position.x,              ViewCam.position.y,              ViewCam.position.z,
//               ViewCam.position.x + ViewCam.lX, ViewCam.position.y + ViewCam.lY, ViewCam.position.z + ViewCam.lZ,
//               ViewCam.upVector.x,              ViewCam.upVector.y,              ViewCam.upVector.z );
//
//    glutSwapBuffers();
//}
//
//void pressKey(int key, int xx, int yy)
//{
//	switch (key) {
//		case GLUT_KEY_UP : ViewCam.deltaMove = 0.25f; break;
//		case GLUT_KEY_DOWN : ViewCam.deltaMove = -0.25f; break;
//	}
//}
//
//void releaseKey(int key, int x, int y)
//{
//	switch (key) {
//		case GLUT_KEY_UP :
//		case GLUT_KEY_DOWN : ViewCam.deltaMove = 0;break;
//	}
//}
//
//void keyPressed(unsigned char key, int x, int y)
//{
//    switch (key)
//    {
//        case  'W':
//        case  'w':
//            ViewCam.deltaMove = 0.125f;
//            break;
//        case  'S':
//        case  's':
//            ViewCam.deltaMove = -0.125f;
//            break;
//        case  'A':
//        case  'a':
//            ViewCam.deltaStrafe = -0.125f;
//            break;
//        case  'D':
//        case  'd':
//            ViewCam.deltaStrafe = 0.125f;
//            break;
//        case  'I':
//        case  'i':
//            DecodeurScene.RealCam.Avance(100.0);
//            break;
//        case  'J':
//        case  'j':
//            DecodeurScene.RealCam.RotateY(45.0);
//            break;
//        case  'K':
//        case  'k':
//            DecodeurScene.RealCam.Avance(-100.0);
//            break;
//        case  'L':
//        case  'l':
//            DecodeurScene.RealCam.RotateY(-45.0);
//            break;
//        case  'P':
//        case  'p':
//            DecodeurScene.updateCloud = true;
//            break;
//        case  '1':
//            DecodeurScene.convertisseur.ConvertiFichier();
//            break;
//        case  '2':
//            CarteVisible = !CarteVisible;
//            break;
//        case  '3':
//            RobotVisible = !RobotVisible;
//            break;
//        case  '4':
//            DecodeurScene.cloudBuffer.EcrirePoint();
//            break;
//        case  '5':
//            DecodeurScene.convertisseur.Environnement.SaveCarte();
//            break;
//        case  'Q':
//        case  'q':
//        case 0x1B:  // ESC
//
//            DecodeurScene.convertisseur.ContinuerConvertion = false;
//            glutDestroyWindow(window);
//
//            exit(0);
//    }
//    //DecodeurScene.RealCam.DebugInfo();
//}
//
//void keyUp(unsigned char key, int x, int y)
//{
//    switch (key)
//    {
//        case  'W':
//        case  'w':
//        case  'S':
//        case  's':
//            ViewCam.deltaMove = 0.0;
//            break;
//        case  'A':
//        case  'a':
//        case  'D':
//        case  'd':
//            ViewCam.deltaStrafe = 0.0;
//            break;
//    }
//}
//
//void mouseMove(int x, int y)
//{
//    if (mx >= 0 && my >= 0)
//    {
//        ViewCam.angleX += (x - mx) * 0.005;
//        ViewCam.lX = sin(ViewCam.angleX);
//        ViewCam.lZ = -cos(ViewCam.angleX);
//
//        ViewCam.angleY += (y - my) * 0.005;
//
//        ViewCam.lY = -sin(ViewCam.angleY);
//        ViewCam.upVector.y = cos(ViewCam.angleY);
//    }
//
//    mx = x;
//    my = y;
//}
//
//void mouseButton(int button, int state, int x, int y)
//{
//    if (state == GLUT_DOWN)
//    {
//        switch (button)
//        {
//            case GLUT_LEFT_BUTTON:
//                mx = x;
//                my = y;
//                break;
//
//            case 3:
//                ViewCam.zoom *= 1.2f;
//                break;
//
//            case 4:
//                ViewCam.zoom /= 1.2f;
//                break;
//        }
//    }
//    else if (state == GLUT_UP && button == GLUT_LEFT_BUTTON)
//    {
//        mx = -1;
//        my = -1;
//    }
//}
//
//void resizeGLScene(int width, int height)
//{
//    glViewport(0, 0, width, height);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(50.0, (float)width / height, 90.0, 110000.0);
//
//    glMatrixMode(GL_MODELVIEW);
//}
//
//void idleGLScene()
//{
//    DecodeurScene.RunLoop();
//    ViewCam.Update();
//    glutPostRedisplay();
//}
//
//void printInfo()
//{
//    std::cout << "\nAvailable Controls:"              << std::endl;
//    std::cout << "==================="                << std::endl;
//    std::cout << "Rotate       :   Mouse Left Button" << std::endl;
//    std::cout << "Zoom         :   Mouse Wheel"       << std::endl;
//    std::cout << "Move cam     :   W A S D"           << std::endl;
//    std::cout << "Move Kinect  :   I J K L"           << std::endl;
//    std::cout << "take a shot  :   P"                 << std::endl;
//    std::cout << "Converti f.  :   1"                 << std::endl;
//    std::cout << "Toggle carte :   2"                 << std::endl;
//    std::cout << "Toggle robot :   3"                 << std::endl;
//    std::cout << "Ecrire points:   4"                 << std::endl;
//    std::cout << "Save Carte   :   5"                 << std::endl;
//    std::cout << "Quit         :   Q or Esc"          << std::endl;
//}
//
//int main(int argc, char **argv)
//{
//    DecodeurScene.Init(freenect.createDevice<MyFreenectDevice>(0));
//    glutInit(&argc, argv);
//    InitRobotMesh();
//    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
//    glutInitWindowSize(IR_CAMERA_RESOLUTION_X, IR_CAMERA_RESOLUTION_Y);
//    glutInitWindowPosition(0, 0);
//
//    window = glutCreateWindow("LibFreenect");
//    glClearColor(0.45f, 0.45f, 0.45f, 0.0f);
//
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_ALPHA_TEST);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glAlphaFunc(GL_GREATER, 0.0f);
//
//    glMatrixMode(GL_PROJECTION);
//    gluPerspective(50.0, 1.0, 900.0, 11000.0);
//
//    glutDisplayFunc(&DrawGLScene);
//    glutIdleFunc(&idleGLScene);
//    glutReshapeFunc(&resizeGLScene);
//    glutKeyboardFunc(&keyPressed);
//    glutKeyboardUpFunc(&keyUp);
//
//	glutIgnoreKeyRepeat(1);
//	glutSpecialFunc(pressKey);
//	glutSpecialUpFunc(releaseKey);
//
//    glutMouseFunc(&mouseButton);
//    glutMotionFunc(&mouseMove);
//
//    printInfo();
//
//    glutMainLoop();
//
//    return 0;
//}
