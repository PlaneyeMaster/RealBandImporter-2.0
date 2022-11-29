// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <string>

using namespace std;

 #ifdef  REALBAND_EXPORTS 
    /*Enabled as "export" while compiling the dll project*/
    #define REALBAND __declspec(dllexport)  
 #else
    /*Enabled as "import" in the Client side for using already created dll file*/
    #define REALBAND __declspec(dllimport)  
 #endif

static uint16 AssetFormat = 0;
static uint64 Texture = 0;
//static uint16 Texture = 0;

struct  UserPreference
{
    string FolderPath;
    string ConnectPath;
    string Project;
    uint16 ActiveTypeBitset;
    uint16 ActiveTextypeBitset;
    uint8 Format;
    int  TexLevel;
    int MeshLevel;
    int TextureType;
};

    enum SELECTOPTIONS
    {
        //SNONE = 0x00,
        FORMAT_GLM = 0x01,
        FORMAT_FBX,
        FORMAT_OBJ,
        HIGH,
        LOW,
        TWO_K,
        FOUR_K,
        EIGHT_K,
        DIFFUSE,
        NORMAL
    };

    enum TEXTUREOPTIONS
    {
        TEXHIGH,
        TEXLOW,
        TWOK,
        FOURK,
        EIGHTK,
        TDIFFUSE,
        TNORMAL
    };

    enum FORMATOPTIONS
    {
        MESH ,
        TEXTURES 
    };


    // Format Regular Expression Array
static char* FormatExprArray[] = {
            "^[0-9]+_mesh_(fbx|obj|glb)_low_\\w+\\.(?:fbx|glb|obj)",
            "^[0-9]+_mesh_(fbx|obj|glb)_high_\\w+\\.(?:fbx|glb|obj)",
            "^[0-9]+_mesh_(fbx|obj|glb)_(low|high)_\\w+\\.(?:fbx|glb|obj)",
            "^[0-9]+_mesh_(obj)_(low|high)_\\w+\\.(obj)",
            "^[0-9]+_mesh_(obj)_(low)_\\w+\\.(obj)",
            "^[0-9]+_mesh_(obj)_(high)_\\w+\\.(obj)",
            "^[0-9]+_mesh_(obj)_(low|high)_\\w+\\.(obj)",
            //"^[0-9]+_mesh_(glb)_(low|high)_\\w+\\.(glb)",
            "^[0-9]+_mesh_(glb)_(high|low)_\\w+\\.(glb)",
            "^[0-9]+_mesh_(glb)_(low)_\\w+\\.(glb)",
            "^[0-9]+_mesh_(glb)_(high)_\\w+\\.(glb)",
            "^[0-9]+_mesh_(glb)_(high|low)_\\w+\\.(glb)",
            "^[0-9]+_mesh_(glb|obj)_(high|low)_\\w+\\.(glb|obj)",
            "^[0-9]+_mesh_(glb|obj)_(low)_\\w+\\.(glb|obj)",
            "^[0-9]+_mesh_(glb|obj)_(high)_\\w+\\.(glb|obj)",
            "^[0-9]+_mesh_(glb|obj)_(high|low)_\\w+\\.(glb|obj)",
            "^[0-9]+_mesh_(fbx)_(high|low)_\\w+\\.(fbx)",
            "^[0-9]+_mesh_(fbx)_(low)_\\w+\\.(fbx)",
            "^[0-9]+_mesh_(fbx)_(high)_\\w+\\.(fbx)",
            "^[0-9]+_mesh_(fbx)_(high|low)_\\w+\\.(fbx)",
            "^[0-9]+_mesh_(fbx|obj)_(high|low)_\\w+\\.(fbx|obj)",
            "^[0-9]+_mesh_(fbx|obj)_(low)_\\w+\\.(fbx|obj)",
            "^[0-9]+_mesh_(fbx|obj)_(high)_\\w+\\.(fbx|obj)",
            "^[0-9]+_mesh_(fbx|obj)_(high|low)_\\w+\\.(fbx|obj)",
            "^[0-9]+_mesh_(fbx|glb)_(high|low)_\\w+\\.(fbx|glb)",
            "^[0-9]+_mesh_(fbx|glb)_(low)_\\w+\\.(fbx|glb)",
            "^[0-9]+_mesh_(fbx|glb)_(high)_\\w+\\.(fbx|glb)",
            "^[0-9]+_mesh_(fbx|glb)_(high|low)_\\w+\\.(fbx|glb)",
            "^[0-9]+_mesh_(fbx|glb|obj)_(high|low)_\\w+\\.(fbx|glb|obj)",
            "^[0-9]+_mesh_(fbx|glb|obj)_(low)_\\w+\\.(fbx|glb|obj)",
            "^[0-9]+_mesh_(fbx|glb|obj)_(high)_\\w+\\.(fbx|glb|obj)",
            "^[0-9]+_mesh_(fbx|glb|obj)_(high|low)_\\w+\\.(fbx|glb|obj)"
};


    // Texture Regular Expression Array
static char* TextureExprArray[] = {
       "",
        "^[0-9]+_texture_png_(diffuse1|normal)_(2k|4k|8k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse1|normal)_(2k|4k|8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse1|normal)_(2k|4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(2k|4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(2k|4k|8k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(2k|4k|8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(2k|4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(2k|4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(2k|4k|8k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(2k|4k|8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(2k|4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k|8k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k|8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(8k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(8k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(8k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(8k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(4k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(4k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(4k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(4k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(4k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(4k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(4k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(4k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(4k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(4k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(4k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(4k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(4k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(4k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(4k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(4k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(4k|8k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(4k|8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(4k|8k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(4k|8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(4k|8k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(4k|8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(4k|8k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(4k|8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(2k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(2k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(2k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(2k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(2k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(2k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(2k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(2k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|8k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(2k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(2k|8k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(2k|8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(2k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(2k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(2k|8k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(2k|8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(2k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|8k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(2k|4k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(2k|4k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(2k|4k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(2k|4k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(2k|4k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(2k|4k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(2k|4k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(2k|4k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k|8k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k|8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(2k|4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(2k|4k|8k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(2k|4k|8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(normal)_(2k|4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(2k|4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k|8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k|8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse)_(2k|4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k|8k)_(high|low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k|8k)_(low)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k|8k)_(high)_\\w+\\.(png)",
        "^[0-9]+_texture_png_(diffuse|normal)_(2k|4k|8k)_(high|low)_\\w+\\.(png)"
};


typedef struct UserPreference USRPREFERENCE;