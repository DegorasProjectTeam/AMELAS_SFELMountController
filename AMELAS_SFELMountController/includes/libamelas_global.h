/***********************************************************************************************************************
 *   AMELAS_SFELMountController: [...].
 *
 *   Copyright (C) 2023 ROA Team (Royal Institute and Observatory of the Spanish Navy)
 *                      < Ángel Vera Herrera, avera@roa.es - angeldelaveracruz@gmail.com >
 *                      < Jesús Relinque Madroñal >
 *                      AVS AMELAS Team
 *                      <>
 *
 *   This file is part of AMELAS_SFELMountController.
 *
 *   Licensed under [...]
 **********************************************************************************************************************/

// =====================================================================================================================
#pragma once
// =====================================================================================================================

// =====================================================================================================================

// Library Version
#define LIBAMELASINTERFACE_VERSION_MAJOR 1
#define LIBAMELASINTERFACE_VERSION_MINOR 1
#define LIBAMELASINTERFACE_VERSION_PATCH 1

// Windows DLL export/import directives
#if ((defined __WIN32__) || (defined _WIN32)) && (!defined LIBAMELASINTERFACE_STATIC)
#ifdef LIBAMELASINTERFACE_LIBRARY
#define LIBAMELAS_EXPORT	__declspec(dllexport)
#define LIBAMELAS_EXPIMP_TEMPLATE
#else
#define LIBAMELASINTERFACE_EXPORT	__declspec(dllimport)
#define LIBAMELAS_EXPIMP_TEMPLATE extern
#endif
#else
/* Static libraries or non-Windows needs no special declaration. */
# define LIBAMELASINTERFACE_EXPORT
#endif

// =====================================================================================================================
