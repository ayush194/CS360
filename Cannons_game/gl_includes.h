//
//  gl_includes.h
//  CS360
//
//  Created by Ayush Kumar on 28/08/2019.
//  Copyright © 2019 Ayush Kumar. All rights reserved.
//

#ifndef gl_includes_h
#define gl_includes_h

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#endif /* gl_includes_h */
