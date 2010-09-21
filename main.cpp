/* 
 * File:   main.cpp
 * Author: ethan
 *
 * Created on 14 September 2010, 4:55 PM
 */

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>

using namespace std;

Display *display;
Window root;
Window win;
GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
XVisualInfo *xVisualInfo;
Colormap colorMap;
XSetWindowAttributes xSWA;
GLXContext glxContext;
XWindowAttributes xWA;
XEvent xEvent;

bool setup()
{
    //Connect to XServer on local machine
    display = XOpenDisplay(NULL);
    //If get error on this line (above) when compiling then add
    //-lX11 -lGL -lGLU
    //to c++ complier additional options

    if(display == NULL)//Check if connection was successful
    {
        cout << "Unable to connect to XServer" << endl;
        return false;
    }

    root = DefaultRootWindow(display);
    xVisualInfo = glXChooseVisual(display, 0, att);
    if(xVisualInfo == NULL)
    {
        cout << "No appropriate visual found" << endl;
        return false;
    }
    else
    {
        cout << "Selected visual " << xVisualInfo->visualid << endl;
    } 
    colorMap = XCreateColormap(display,root, xVisualInfo->visual, AllocNone);
    xSWA.colormap = colorMap;
    xSWA.event_mask = ExposureMask | KeyPressMask;
    win = XCreateWindow(display,
                        root,
                        0,
                        0,
                        600,
                        600,
                        0,
                        xVisualInfo->depth,
                        InputOutput,
                        xVisualInfo->visual,
                        CWColormap | CWEventMask,
                        &xSWA);
    XMapWindow(display, win);
    XStoreName(display, win, "Open GL test2");
    glxContext = glXCreateContext(display, xVisualInfo, NULL, GL_TRUE);
    glXMakeCurrent(display, win, glxContext);
    glEnable(GL_DEPTH_TEST);
    
    return true;//Setup is successful
}

void draw()
{
    //Clear window for redraw
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, 20.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(/*Camera pos*/0.0, 0.0, 10.0,
              /*Camera focus*/0.0, 0.0, 0.0,
              /*Up direction*/0.0, 1.0, 0.0);

    glBegin(GL_TRIANGLES);

    glColor3f(0.0, 0.0, 1.0)/*Blue*/;glVertex3f(-0.75, -0.75, 0.0);
    glColor3f(1.0, 0.0, 0.0)/*Red*/;glVertex3f(-0.75, 0.0, 0.0);
    glColor3f(1.0, 1.0, 0.0)/*Yellow*/;glVertex3f(0.0, -0.75, 0.0);

    glColor3f(1.0, 0.0, 0.0)/*Red*/;glVertex3f(0.75, -0.75, 0.0);
    glColor3f(1.0, 0.5, 0.0)/*Orange*/;glVertex3f(0.75, 0.0, 0.0);
    glColor3f(1.0, 1.0, 0.0)/*Yellow*/;glVertex3f(0.0, -0.75, 0.0);

    glColor3f(1.0, 1.0, 1.0)/*White*/;glVertex3f(-0.75, 0.75, 0.0);
    glColor3f(1.0, 0.0, 0.0)/*Red*/;glVertex3f(-0.75, 0.0, 0.0);
    glColor3f(0.0, 1.0, 0.0)/*Green*/;glVertex3f(0.0, 0.75, 0.0);

    glColor3f(1.0, 0.0, 0.0)/*Red*/;glVertex3f(0.75, 0.75, 0.0);
    glColor3f(1.0, 0.5, 0.0)/*Orange*/;glVertex3f(0.75, 0.0, 0.0);
    glColor3f(1.0, 1.0, 0.0)/*Yellow*/;glVertex3f(0.0, 0.75, 0.0);

    glEnd();
}

/*
 * 
 */
int main(int argc, char** argv)
{
    if(setup())
    {
        while(1)
        {
            XNextEvent(display, &xEvent);

            if(xEvent.type == Expose)
            {
                //Draw the window

                XGetWindowAttributes(display, win, &xWA);
                glViewport(0,0, xWA.width, xWA.height);
                draw();
                glXSwapBuffers(display, win);
            }
            else if(xEvent.type == KeyPress)
            {
                //Close the window

                glXMakeCurrent(display, None, NULL);
                glXDestroyContext(display,glxContext);
                XDestroyWindow(display, win);
                XCloseDisplay(display);
                return 0;
            }
        }
    }
    else
    {
        return 0;//Unable to setup display window
    }

    return 0;//No errors
}


