// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * [[[FILE NAME]]]
 *
 * [[[BREIF DESCRIPTION]]]
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	1.0.0
 */

#include "expExporterBase.h"
#include "time.h"
#include <sstream>
#include <fstream>

FILE* sLogFile = nullptr;

FILE* GetLogFile()
{
  return sLogFile;
}

void OpenLogFile(const char* name)
{

  std::stringstream ss;
  ss << name << ".log";

#if STEXP_PLATFORM == STEXP_PLATFORM_WINDOWS
  fopen_s(&sLogFile, ss.str().c_str(), "w+");
#else
  sLogFile = fopen(ss.str().c_str(), "w+");
#endif

  std::ofstream s;
  s.open("midi.txt");
  s << sLogFile << std::endl;
  s.close();

  if (sLogFile == NULL)
  {
    int t = (int) time(NULL);

    ss.clear();
    ss << name << '_' << t << ".log";

#if STEXP_PLATFORM == STEXP_PLATFORM_WINDOWS
    fopen_s(&sLogFile, ss.str().c_str(), "w+");
#else
    sLogFile = fopen(ss.str().c_str(), "w+");
#endif
  }


}

void CloseLogFile()
{
  if (sLogFile)
  {
    fclose(sLogFile);
    sLogFile = nullptr;
  }
}
