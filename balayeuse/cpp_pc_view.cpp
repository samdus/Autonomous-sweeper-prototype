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
//#include <math.h>
//#include "SceneCamera.h"
//#include "Decodeur.h"
//#include "Vectors.h"
//#include "CloudPointContainer.h"
//
//#if defined(__APPLE__)
//#include <GLUT/glut.h>
//#else
//#include <GL/glut.h>
//#endif
//
//Freenect::Freenect freenect;
//int window(0);                          // Glut window identifier
//int mx = -1, my = -1;                   // Previous mouse coordinates
//bool color = true;                      // Flag to indicate to use of color in the cloud
//SceneCamera ViewCam = SceneCamera();    // Camera to navigate inside the generated map
//Decodeur DecodeurScene = Decodeur();
//
//void DrawGLScene()
//{
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    glPointSize(2.0f);
//
//    glBegin(GL_POINTS);
//
//    std::vector<uint8_t> currentRgb;
//    std::vector<Vector3> currentDepth;
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
//            for (int i = 0; i < IR_CAMERA_RESOLUTION_Y*IR_CAMERA_RESOLUTION_X; ++i)
//            {
//                if (color)
//                    glColor3ub( currentRgb[3*i+0],    // R
//                                currentRgb[3*i+1],    // G
//                                currentRgb[3*i+2]);   // B
//
//                glVertex3f(currentDepth[i].x, currentDepth[i].y, currentDepth[i].z);
//            }
//        }
//
//    }
//    glEnd();
//
//    // Draw the world coordinate frame
//    glLineWidth(2.0f);
//    glBegin(GL_LINES);
//    glColor3ub(255, 0, 0);  // X-axis
//    glVertex3f(  0, 0, 0);
//    glVertex3f( 50, 0, 0);
//    glColor3ub(0, 255, 0);  // Y-axis
//    glVertex3f(0,   0, 0);
//    glVertex3f(0,  50, 0);
//    glColor3ub(0, 0, 255);  // Z-axis
//    glVertex3f(0, 0,   0);
//    glVertex3f(0, 0,  50);
//    glEnd();
//
//    //Draw a plane
//    glBegin(GL_TRIANGLES);
//		glVertex3f(0.0,0.0,0.0);
//		glVertex3f(500.0,0.0,0.0);
//		glVertex3f(0.0,0.0,500.0);
//
//        glVertex3f(0.0,0.0,500.0);
//		glVertex3f(500.0,0.0,500.0);
//		glVertex3f(500.0,0.0,0.0);
//	glEnd();
//
//    // Place the camera
//    //glMatrixMode(GL_MODELVIEW);
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
//		case GLUT_KEY_UP : ViewCam.deltaMove = 0.5f; break;
//		case GLUT_KEY_DOWN : ViewCam.deltaMove = -0.5f; break;
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
//            ViewCam.deltaMove = 0.5f;
//            break;
//        case  'S':
//        case  's':
//            ViewCam.deltaMove = -0.5f;
//            break;
//        case  'A':
//        case  'a':
//            ViewCam.deltaStrafe = 0.5f;
//            break;
//        case  'D':
//        case  'd':
//            ViewCam.deltaStrafe = -0.5f;
//            break;
//        case  'C':
//        case  'c':
//            color = !color;
//            break;
//
//        case  'Q':
//        case  'q':
//        case 0x1B:  // ESC
//            glutDestroyWindow(window);
//            exit(0);
//    }
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
//        ViewCam.angleX -= (x - mx) * 0.005;
//        ViewCam.lX = sin(ViewCam.angleX);
//        ViewCam.lZ = -cos(ViewCam.angleX);
//
//        ViewCam.angleY -= (y - my) * 0.005;
//
//        ViewCam.lY = -sin(ViewCam.angleY);
//        ViewCam.upVector.y = -cos(ViewCam.angleY);
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
//    gluPerspective(50.0, (float)width / height, 900.0, 11000.0);
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
//    std::cout << "Toggle Color :   C"                 << std::endl;
//    std::cout << "Quit         :   Q or Esc\n"        << std::endl;
//}
//
//int main(int argc, char **argv)
//{
//    DecodeurScene.Init(freenect.createDevice<MyFreenectDevice>(0));
//    glutInit(&argc, argv);
//
//    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
//    glutInitWindowSize(IR_CAMERA_RESOLUTION_X, IR_CAMERA_RESOLUTION_Y);
//    glutInitWindowPosition(0, 0);
//
//    window = glutCreateWindow("LibFreenect");
//    glClearColor(0.45f, 0.45f, 0.45f, 0.0f);
//
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_ALPHA_TEST);
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
