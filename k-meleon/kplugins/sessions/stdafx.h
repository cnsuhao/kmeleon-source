// stdafx.h : Fichier Include pour les fichiers Include syst�me standard,
// ou les fichiers Include sp�cifiques aux projets qui sont utilis�s fr�quemment,
// et sont rarement modifi�s
//

#pragma once

#define _WIN32_WINNT 0x0500
#define WIN32_LEAN_AND_MEAN		// Exclure les en-t�tes Windows rarement utilis�s

#include <windows.h>
#include <stdlib.h>
#define KMELEON_PLUGIN_EXPORTS
#include "kmeleon_plugin.h"
#include "../../app/kmeleonconst.h"
#include "utils.h"
#include "LocalesUtils.h"
#include "strconv.h"

// This limit to 100 the number of saved session
#define MAX_SAVED_SESSION 100


extern kmeleonPlugin kPlugin;

std::string itos(int i);
void setIntPref(const char* prefname, int value, bool flush = FALSE);
void setStrPref(const char* prefname, char* value, bool flush = FALSE);
int getIntPref(const char* prefname, int defvalue);
std::string getStrPref(const char* prefname, const char* defvalue);
