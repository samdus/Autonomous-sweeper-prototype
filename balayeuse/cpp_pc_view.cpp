/*
 * This file is part of the OpenKinect Project. http://www.openkinect.org
 *
 * Copyright (c) 2010 individual OpenKinect contributors. See the CONTRIB file
 * for details.
 *
 * This code is licensed to you under the terms of the Apache License, version
 * 2.0, or, at your option, the terms of the GNU General Public License,
 * version 2.0. See the APACHE20 and GPL2 files for the text of the licenses,
 * or the following URLs:
 * http://www.apache.org/licenses/LICENSE-2.0
 * http://www.gnu.org/licenses/gpl-2.0.txt
 *
 * If you redistribute this file in source form, modified or unmodified, you
 * may:
 *   1) Leave this header intact and distribute it under the same terms,
 *      accompanying it with the APACHE20 and GPL20 files, or
 *   2) Delete the Apache 2.0 clause and accompany it with the GPL2 file, or
 *   3) Delete the GPL v2 clause and accompany it with the APACHE20 file
 * In all cases you must keep the copyright notice intact and include a copy
 * of the CONTRIB file.
 *
 * Binary distributions must follow the binary distribution requirements of
 * either License.
 */

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <pthread.h>
#include "libfreenect.hpp"

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define IR_CAMERA_RESOLUTION_X 640
#define IR_CAMERA_RESOLUTION_Y 480
#define CLOUD_POINT_CIRCULAR_BUFFER 10
#define CLOUD_POINT_SAMPLING_FREQUENCY 500 //millisecond

class CircularCloudPoint
{
    private:
    int index = 0;
    std::vector<uint16_t> depth[CLOUD_POINT_CIRCULAR_BUFFER];
    //std::vector<uint16_t> depth(IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y * CLOUD_POINT_CIRCULAR_BUFFER);

    public:
    void insert(std::vector<uint16_t> buffer)//we need a copy
    {
        if(index >= CLOUD_POINT_CIRCULAR_BUFFER)
            index = 0;
        depth[index] = buffer;
        ++index;
    }

    /*std::vector<uint16_t> GetCloudPoint()const
    {
        return depth;
    }*/
};

class Mutex
{
public:
    Mutex()
    {
        pthread_mutex_init(&m_mutex, NULL);
    }

    void lock()
    {
        pthread_mutex_lock(&m_mutex);
    }

    void unlock()
    {
        pthread_mutex_unlock(&m_mutex);
    }

    class ScopedLock
    {
    public:
        ScopedLock(Mutex &mutex) : _mutex(mutex)
        {
            _mutex.lock();
        }

        ~ScopedLock()
        {
            _mutex.unlock();
        }

    private:
        Mutex &_mutex;
    };

private:
    pthread_mutex_t m_mutex;
};


class MyFreenectDevice : public Freenect::FreenectDevice
{
public:
    MyFreenectDevice(freenect_context *_ctx, int _index)
        : Freenect::FreenectDevice(_ctx, _index),
          m_buffer_video(freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB).bytes),
          m_buffer_depth(freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_MM).bytes / 2),
          m_new_rgb_frame(false), m_new_depth_frame(false)
    {
        setDepthFormat(FREENECT_DEPTH_REGISTERED);
    }

    // Do not call directly, even in child
    void VideoCallback(void *_rgb, uint32_t timestamp)
    {
        Mutex::ScopedLock lock(m_rgb_mutex);
        uint8_t* rgb = static_cast<uint8_t*>(_rgb);
        copy(rgb, rgb+getVideoBufferSize(), m_buffer_video.begin());
        m_new_rgb_frame = true;
    }

    // Do not call directly, even in child
    void DepthCallback(void *_depth, uint32_t timestamp)
    {
        Mutex::ScopedLock lock(m_depth_mutex);
        uint16_t* depth = static_cast<uint16_t*>(_depth);
        copy(depth, depth+getDepthBufferSize()/2, m_buffer_depth.begin());
        m_new_depth_frame = true;
    }

    bool getRGB(std::vector<uint8_t> &buffer)
    {
        Mutex::ScopedLock lock(m_rgb_mutex);

        if (!m_new_rgb_frame)
            return false;

        buffer.swap(m_buffer_video);
        m_new_rgb_frame = false;

        return true;
    }

    bool getDepth(std::vector<uint16_t> &buffer)
    {
        Mutex::ScopedLock lock(m_depth_mutex);

        if (!m_new_depth_frame)
            return false;

        buffer.swap(m_buffer_depth);
        m_new_depth_frame = false;

        return true;
    }

private:
    Mutex m_rgb_mutex;
    Mutex m_depth_mutex;
    std::vector<uint8_t> m_buffer_video;
    std::vector<uint16_t> m_buffer_depth;
    bool m_new_rgb_frame;
    bool m_new_depth_frame;
};


Freenect::Freenect freenect;
MyFreenectDevice* device;
CircularCloudPoint cloudBuffer;

int window(0);                // Glut window identifier
int mx = -1, my = -1;         // Prevous mouse coordinates
float anglex = 0, angley = 0; // Panning angles
float zoom = 1;               // Zoom factor
bool color = true;            // Flag to indicate to use of color in the cloud

int frame = 0;
bool updateFPS = true;
double starttime = 0;
float fps;

std::vector<uint8_t> rgb(IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y*3);
std::vector<uint16_t> depth(IR_CAMERA_RESOLUTION_X*IR_CAMERA_RESOLUTION_Y);


void DrawGLScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPointSize(1.0f);

    glBegin(GL_POINTS);

    if (!color) glColor3ub(255, 255, 255);
    for (int i = 0; i < IR_CAMERA_RESOLUTION_Y*IR_CAMERA_RESOLUTION_X; ++i)
    {
        if (color)
            glColor3ub( rgb[3*i+0],    // R
                        rgb[3*i+1],    // G
                        rgb[3*i+2] );  // B

        float f = 595.f;
        // Convert from image plane coordinates to world coordinates
        glVertex3f( (i%IR_CAMERA_RESOLUTION_X - (IR_CAMERA_RESOLUTION_X-1)/2.f) * depth[i] / f,  // X = (x - cx) * d / fx
                    (i/IR_CAMERA_RESOLUTION_X - (IR_CAMERA_RESOLUTION_Y-1)/2.f) * depth[i] / f,  // Y = (y - cy) * d / fy
                    depth[i] );                            // Z = d
    }

    glEnd();

    // Draw the world coordinate frame
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glColor3ub(255, 0, 0);  // X-axis
    glVertex3f(  0, 0, 0);
    glVertex3f( 50, 0, 0);
    glColor3ub(0, 255, 0);  // Y-axis
    glVertex3f(0,   0, 0);
    glVertex3f(0,  50, 0);
    glColor3ub(0, 0, 255);  // Z-axis
    glVertex3f(0, 0,   0);
    glVertex3f(0, 0,  50);
    glEnd();

    //Draw a plane
    glBegin(GL_TRIANGLES);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(500.0,0.0,0.0);
		glVertex3f(0.0,0.0,500.0);

        glVertex3f(0.0,0.0,500.0);
		glVertex3f(500.0,0.0,500.0);
		glVertex3f(500.0,0.0,0.0);
	glEnd();

    // Place the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(zoom, zoom, 1);
    gluLookAt( -7*anglex, -7*angley, -1000.0,
                     0.0,       0.0,  2000.0,
                     0.0,      -1.0,     0.0 );

    glutSwapBuffers();
}


void keyPressed(unsigned char key, int x, int y)
{
    switch (key)
    {
        case  'C':
        case  'c':
            color = !color;
            break;

        case  'Q':
        case  'q':
        case 0x1B:  // ESC
            glutDestroyWindow(window);
            device->stopDepth();
            device->stopVideo();
            exit(0);
    }
}


void mouseMoved(int x, int y)
{
    if (mx >= 0 && my >= 0)
    {
        anglex += x - mx;
        angley += y - my;
    }

    mx = x;
    my = y;
}


void mouseButtonPressed(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        switch (button)
        {
            case GLUT_LEFT_BUTTON:
                mx = x;
                my = y;
                break;

            case 3:
                zoom *= 1.2f;
                break;

            case 4:
                zoom /= 1.2f;
                break;
        }
    }
    else if (state == GLUT_UP && button == GLUT_LEFT_BUTTON)
    {
        mx = -1;
        my = -1;
    }
}


void resizeGLScene(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, (float)width / height, 900.0, 11000.0);

    glMatrixMode(GL_MODELVIEW);
}


void UpdateFPS(bool showFpsConsole, bool showFpsInScene)
{
    if(updateFPS)
    {
        starttime = time(0);
        frame = 0;
        updateFPS = false;
    }
    else
    {
        frame++;
        double now = time(0);
        if(now - 1 >= starttime)
        {
            updateFPS = true;

            if(showFpsConsole)
                std::cout << "fps: " << frame << std::endl;
        }
    }
}

void UpdateCloudOfPoint()
{
    device->getRGB(rgb);
    device->getDepth(depth);

    cloudBuffer.insert(depth);
}

void idleGLScene()
{
    UpdateFPS(true,false);
    UpdateCloudOfPoint();

    glutPostRedisplay();
}


void printInfo()
{
    std::cout << "\nAvailable Controls:"              << std::endl;
    std::cout << "==================="                << std::endl;
    std::cout << "Rotate       :   Mouse Left Button" << std::endl;
    std::cout << "Zoom         :   Mouse Wheel"       << std::endl;
    std::cout << "Toggle Color :   C"                 << std::endl;
    std::cout << "Quit         :   Q or Esc\n"        << std::endl;
}


int main(int argc, char **argv)
{
    device = &freenect.createDevice<MyFreenectDevice>(0);
    device->startVideo();
    device->startDepth();

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(IR_CAMERA_RESOLUTION_X, IR_CAMERA_RESOLUTION_Y);
    glutInitWindowPosition(0, 0);

    window = glutCreateWindow("LibFreenect");
    glClearColor(0.45f, 0.45f, 0.45f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(50.0, 1.0, 900.0, 11000.0);

    glutDisplayFunc(&DrawGLScene);
    glutIdleFunc(&idleGLScene);
    glutReshapeFunc(&resizeGLScene);
    glutKeyboardFunc(&keyPressed);
    glutMotionFunc(&mouseMoved);
    glutMouseFunc(&mouseButtonPressed);

    printInfo();

    glutMainLoop();

    return 0;
}
