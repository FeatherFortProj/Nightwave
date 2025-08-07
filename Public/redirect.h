#pragma once

extern "C" {
    
typedef void* (*CurlEasySetOptFn)(void* curl, int option, void* arg);
extern CurlEasySetOptFn OGCurlEasySetOpt;

}
