/******************************************************************/
/*!
\file      WindowFile.h
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 30, 2024
\brief     This file contains the declaration to open the file explorer.


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef WINFILE_H
#define WINFILE_H


#include <Windows.h>
#include <commdlg.h>
#include <vector>
#include <string>

namespace file {

	class FileWindow {

	public:

		/******************************************************************/
		/*!
		\fn      static std::string file::FileWindow::m_OpenfileDialog(char* filepath)
		\brief   Opens a file dialog for selecting a file.
		\param   filepath - Pointer to a character array to store the selected file path.
		\return  The path of the selected file as a string.
		\details Displays a Windows file dialog allowing the user to select a file, then stores the selected path.
		*/
		/******************************************************************/
		static std::string m_OpenfileDialog(char* filepath);

		/******************************************************************/
		/*!
		\fn      static std::vector<std::string> file::FileWindow::readEditorConfig(const std::string& filePath)
		\brief   Reads configuration data from an editor config file.
		\param   filePath - Path to the editor configuration file.
		\return  A vector of strings containing the configuration data.
		\details Parses the editor configuration file and returns the data for further use.
		*/
		/******************************************************************/
		static std::vector<std::string> readEditorConfig(const std::string& filePath);

		//static std::string m_BackupAndDelete(const std::filesystem::path& path);

	};



}

#endif WINFILE_H