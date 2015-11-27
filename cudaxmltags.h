/****************************************************************************************
 *	Copyright (c) 2015, Jose Gerardo Palma Duran
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without modification,
 *	are permitted provided that the following conditions are met:
 *
 *	1. Redistributions of source code must retain the above copyright notice,
 *	   this list of conditions and the following disclaimer.
 *
 *	2. Redistributions in binary form must reproduce the above copyright notice, this
 *	   list of conditions and the following disclaimer in the documentation and/or
 *	   other materials provided with the distribution.
 *
 *	 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *	 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *	 IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 *	 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *	 NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *	 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *	 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *	 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *	 POSSIBILITY OF SUCH DAMAGE.
 ****************************************************************************************/
#ifndef CUDASON_CUDAXMLTAGS_H
#define CUDASON_CUDAXMLTAGS_H

// Tags used in the xml parsing process

#define CUDA_XMLTAGS_CUDASON "cudason"
#define CUDA_XMLTAGS_CLASS "class"
#define CUDA_XMLTAGS_STRING "string"
#define CUDA_XMLTAGS_INTEGER "integer"
#define CUDA_XMLTAGS_INTEGER32 "int32"
#define CUDA_XMLTAGS_FLOAT "float"
#define CUDA_XMLTAGS_BOOLEAN "boolean"
#define CUDA_XMLTAGS_LIST "list"
#define CUDA_XMLTAGS_REFCLASS "refclass"
#define CUDA_XMLTAGS_MAP "map"
#define CUDA_XMLTAGS_JSON "json"


// Attributes used in the xml parsing process

#define CUDA_XMLATTR_NAME "name"
#define CUDA_XMLATTR_OPTIONAL "optional"
#define CUDA_XMLATTR_REFCLASS "refclass"
#define CUDA_XMLATTR_CONDITION "condition"
#define CUDA_XMLATTR_VALUE "value"

/**
 * Macro used to store the amount of space we want to use as tabulations
 * No \t is used as that will create code that may be difficult to read
 * depending on the enviroment configuration.
 * */
#define TABS "    "
/**
 * Disclaimer printed on the .h and .cpp files
 * */
#define DISCLAIMER "/**\n\
 * This file has been created automatically. Any change will be lost\n\
 * Please modify your input template to create a different output\n\
 * instead of modifying this file.\n\
 *\n\
 * Author: José Gerardo Palma Durán\n\
 *\n\
 * Disclaimer: The author of this software is not responsible of any\n\
 * possible damage/problem caused by the usage of this software. This\n\
 * code is provided as is, without any warranty.\n\
 *\n\
 * If you want to report any bug, please contact me at jpalma at barracuda dot com\n\
 * \n\
 * If you didn't get a copy of the code used to create these templates, you can always\n\
 * download it for free from https://github.com/raistmaj/xml2json\n\
 * */\n"

#endif //CUDASON_CUDAXMLTAGS_H
