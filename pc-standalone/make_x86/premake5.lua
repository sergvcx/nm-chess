#!lua
ROOT = "../.."
-- A solution contains projects, and defines the available configurations
  solution "test-x86"		 
	configurations { "Debug", "Release" }
	project "test-x86"
      kind "ConsoleApp"
      language "C++"
      files { "../*.cpp","../../*.h","../../src/pc/*.*","../../src/common/*.*" }
	  links { "nmpp-x86","vshell"} 
	  libdirs { "$(NMPP)/lib","$(HAL)/lib","$(VSHELL)/lib"}
	  includedirs { "$(NMPP)/include","$(HAL)/include","../..","$(VSHELL)/include"}
	  
      configuration "Debug"
         defines { "DEBUG" }
         symbols  "On" 
		 
		 

      configuration "Release"
         defines { "NDEBUG" }
         symbols  "Off" 
		 

