#include "Dlg.h"

template<size_t ItemCount>
HANDLE Dlg::CreateDialogTemplate(const DIALOG &Dlg, const std::array<ITEM, ItemCount> &DlgItems) {

	size_t DlgTemplateSize = 0U;

	/* TODO: Calculate Dialog Template Size In Bytes */

	DlgTemplateSize += sizeof(DLGTEMPLATE);

	(Dlg.Menu != NULL) ? DlgTemplateSize += sizeof(WCHAR) * (wcslen(Dlg.Menu) + 1) :
		DlgTemplateSize += sizeof(WORD);

	DlgTemplateSize += sizeof(WORD);

	(Dlg.Title != NULL) ? DlgTemplateSize += sizeof(WCHAR) * (wcslen(Dlg.Title) + 1) :
		DlgTemplateSize += sizeof(WORD);

	for (const ITEM &DlgItem : DlgItems) {
		
		DlgTemplateSize += sizeof(DLGITEMTEMPLATE);

		//(DlgItem.Class != 0x0000) ? DlgTemplateSize += sizeof(WORD) * 2 :
			/* TODO: Assert For Syntax Error */

	}

	//=====================================//
	// ALLOCATE MEMORY FOR DIALOG TEMPLATE //
	//=====================================//
	LPBYTE hDlgTemplate = new (std::nothrow) BYTE[DlgTemplateSize]{};
	if (hDlgTemplate == NULL) {
		return NULL;
	}

	/* TODO: Create Dialog Template */

	//========//
	// DIALOG //
	//========//
	LPDLGTEMPLATE lpDlg = (LPDLGTEMPLATE)(hDlgTemplate);
	lpDlg->style = Dlg.Style;
	lpDlg->dwExtendedStyle = Dlg.ExtendedStyle;
	lpDlg->cdit = ItemCount;
	lpDlg->x = Dlg.X; lpDlg->y = Dlg.Y;
	lpDlg->cx = Dlg.Width; lpDlg->cy = Dlg.Height;
	
	if (Dlg.Menu != NULL) {
		LPWSTR lpMenu = (LPWSTR)(hDlgTemplate + sizeof(LPDLGTEMPLATE));
		wmemcpy(lpMenu, Dlg.Menu, wcslen(Dlg.Menu) + 1); // # Menu Resource #
	} else {
		LPWORD lpMenu = (LPWORD)(hDlgTemplate + sizeof(LPDLGTEMPLATE));
		*lpMenu = 0x0000; // # No Menu #
	}


	//====================//
	// DIALOG ITEMS STYLE //
	//====================//
	for (const ITEM &DlgItem : DlgItems) {



	}


	return hDlgTemplate;

}

INT_PTR Dlg::ShowDialog(HWND hWndParent, HANDLE hDlgTemplate, DLGPROC DlgProc, LPVOID lpParam) {
	
	

	return 0;

}

VOID Dlg::DeleteDialogTemplate(HANDLE hDlgTemplate) {

	delete[] hDlgTemplate;
	hDlgTemplate = NULL;

}
