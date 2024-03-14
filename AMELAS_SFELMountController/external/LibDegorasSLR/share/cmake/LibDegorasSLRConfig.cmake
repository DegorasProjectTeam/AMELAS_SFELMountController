# **********************************************************************************************************************
#    LibDegorasSLR (Degoras Project SLR Library).
#
#    A modern and efficient C++ base library for Satellite Laser Ranging (SLR) software and real-time hardware
#    related developments. Developed as a free software under the context of Degoras Project for the Spanish Navy
#    Observatory SLR station (SFEL) in San Fernando and, of course, for any other station that wants to use it!
#
#    Copyright (C) 2023 Degoras Project Team
#                       < Ángel Vera Herrera, avera@roa.es - angeldelaveracruz@gmail.com >
#                       < Jesús Relinque Madroñal >
#
#    This file is part of LibDPSLR.
#
#    Licensed under the European Union Public License (EUPL), Version 1.2 or subsequent versions of the EUPL license
#    as soon they will be approved by the European Commission (IDABC).
#
#    This project is free software: you can redistribute it and/or modify it under the terms of the EUPL license as
#    published by the IDABC, either Version 1.2 or, at your option, any later version.
#
#    This project is distributed in the hope that it will be useful. Unless required by applicable law or agreed to in
#    writing, it is distributed on an "AS IS" basis, WITHOUT ANY WARRANTY OR CONDITIONS OF ANY KIND; without even the
#    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the EUPL license to check specific
#    language governing permissions and limitations and more details.
#
#    You should use this project in compliance with the EUPL license. You should have received a copy of the license
#    along with this project. If not, see the license at < https://eupl.eu/ >.
# **********************************************************************************************************************



# Find the LibNovasCpp package required by LibDegorasSLR, in this case.
find_package(LibNovasCpp 3.1 REQUIRED PATHS C:/Users/adelmoral/Documents/Proyectos/Pruebas/LibNovasCpp_product_v3.1/mingw-x86_64-13.2.0-release/share/cmake)

# Include the /LibDegorasSLRTargets file.
include("${CMAKE_CURRENT_LIST_DIR}/LibDegorasSLRTargets.cmake")

# **********************************************************************************************************************
