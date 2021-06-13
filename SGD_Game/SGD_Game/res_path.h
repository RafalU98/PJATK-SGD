#ifndef RES_PATH_H
#define RES_PATH_H

#include <iostream>
#include <string>
#include <SDL.h>

//returns the path of subdictioneries to ease up searching ressources
std::string getResourcePath(const std::string &subDir = "");

#endif