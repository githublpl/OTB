/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbWrapperCommandLineLauncher.h"


int otbWrapperCommandLineLauncherNew(int argc, char* argv[])
{
  typedef otb::Wrapper::CommandLineLauncher LauncherType;
  LauncherType::Pointer launcher = LauncherType::New();

  return EXIT_SUCCESS;
}

int otbWrapperCommandLineLauncherTest(int argc, char* argv[])
{
  typedef otb::Wrapper::CommandLineLauncher LauncherType;
  LauncherType::Pointer launcher = LauncherType::New();

  if (launcher->Load( argv[1]) == false )
    {
      if (launcher->ExecuteAndWriteOutput() == true)
        {
          return EXIT_FAILURE;
        }
    }
  else
    {
      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}


