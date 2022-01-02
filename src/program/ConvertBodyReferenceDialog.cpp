/*
BodySlide and Outfit Studio
See the included LICENSE file
*/

#include "ConvertBodyReferenceDialog.h"
#include "../utils/ConfigurationManager.h"
#include "../utils/ConfigDialogUtil.h"

#include <regex>

class RefTemplate;
extern ConfigurationManager Config;

wxBEGIN_EVENT_TABLE(ConvertBodyReferenceDialog, wxDialog)

wxEND_EVENT_TABLE()

ConvertBodyReferenceDialog::ConvertBodyReferenceDialog(wxWindow* parent, ConfigurationManager& config, std::vector<RefTemplate> refTemplates) {
	wxXmlResource* xrc = wxXmlResource::Get();
	xrc->Load(wxString::FromUTF8(Config["AppDir"]) + "/res/xrc/ConvertBodyReference.xrc");
	xrc->LoadDialog(this, parent, "dlgConvertBodyRef");

	wxChoice* choice = XRCCTRL((*this), "npConvRefChoice", wxChoice);
	choice->Append("None");

	ConfigDialogUtil::LoadDialogChoices(config, (*this), "npConvRefChoice", refTemplates);
	ConfigDialogUtil::LoadDialogChoices(config, (*this), "npNewRefChoice", refTemplates);
	ConfigDialogUtil::LoadDialogText(config, (*this), "npRemoveText");
	ConfigDialogUtil::LoadDialogText(config, (*this), "npAppendText");
	ConfigDialogUtil::LoadDialogText(config, (*this), "npDeleteShapesText");
	ConfigDialogUtil::LoadDialogText(config, (*this), "npAddBonesText");
	ConfigDialogUtil::LoadDialogCheckBox(config, (*this), "chkKeepZapSliders");
	ConfigDialogUtil::LoadDialogCheckBox(config, (*this), "chkSkipConformPopup");
	ConfigDialogUtil::LoadDialogCheckBox(config, (*this), "chkSkipCopyBonesPopup");
	ConfigDialogUtil::LoadDialogCheckBox(config, (*this), "chkDeleteReferenceOnComplete");
	ConfigDialogUtil::LoadDialogCheckBox(config, (*this), "chkDeleteUnreferencedNodesOnComplete");

	SetDoubleBuffered(true);
	CenterOnParent();
}
	
ConvertBodyReferenceDialog::~ConvertBodyReferenceDialog() {
	wxXmlResource::Get()->Unload(wxString::FromUTF8(Config["AppDir"]) + "/res/xrc/ConvertBodyReferenceDialog.xrc");
}