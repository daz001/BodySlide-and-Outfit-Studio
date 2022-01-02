/*
BodySlide and Outfit Studio
See the included LICENSE file
*/

#pragma once

#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

class ConfigurationManager;
class RefTemplate;

class ConvertBodyReferenceDialog : public wxDialog {
public:

	ConvertBodyReferenceDialog(wxWindow* parent, ConfigurationManager& config, std::vector<RefTemplate> refTemplates);
	~ConvertBodyReferenceDialog();

	wxDECLARE_EVENT_TABLE();
};
