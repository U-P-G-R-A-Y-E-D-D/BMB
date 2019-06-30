#pragma once


#ifdef _MSC_VER 

#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <iostream>
#include <cmath>
#include <cctype>
#include <cstring>

#ifndef EasyBMP
#define EasyBMP



#ifndef _DefaultXPelsPerMeter_
#define _DefaultXPelsPerMeter_
#define DefaultXPelsPerMeter 3780
// nastavi na vychozi 96 dpi 
#endif

#ifndef _DefaultYPelsPerMeter_
#define _DefaultYPelsPerMeter_
#define DefaultYPelsPerMeter 3780
// nastavvi na vychozi  96 dpi
#endif

#include "BMP_DataStruct.h"
#include "BMP_BMP.h"
#include "BMP_ruzneBMP.h"






#endif