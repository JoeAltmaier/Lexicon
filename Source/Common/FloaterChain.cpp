/* FloaterChain.cpp -- List of FloaterElmt
 *
 * Copyright (c) Tom Nelson, 2004-2010
 * Copyright (c) Iowa Software Design, 2008,2009,2020
 *
 * This material is a confidential trade secret and proprietary information of
 * Iowa Software Design, which may not be reproduced, used, sold or transferred
 * to any third party without the prior written consent of Iowa Software Design.
 * This material is also copyrighted as an unpublished work under sections 104 and
 * 408 of Title 17 of the United States Code. Law prohibits unauthorized use,
 * copying or reproduction.
 *
 * Tom Nelson, PO Box 649, North Liberty,IA 52317, 319-936-6856
 * Iowa Software Design, PO Box 649, North Liberty,IA 52317, 319-626-3077
 *
 * Revision History:
 *      5-07-20 TRN  Created
 *      3-17-23 JCA  Use FontBox
 *
**/

#include "FloaterChain.h"
#include "TColorNames.h"
#include "LexSkins.h"

// .ForgeFloaterElmt
//
FloaterElmt *FloaterChain::ForgeFloaterElmt(int _tStyle, CPoint _ptElmt, const char *_pszText)
{
    FloaterElmt *pFloaterElmt = new FloaterElmt;

    SFontBox::Context context(&bmFontStrip, svLex.svtextBonusScoreFont.pString, svLex.svrgbTransparent.color);

    if (pFloaterElmt->Create(_tStyle | SElement::esNOPARENTCLIP, CRect(_ptElmt,CSize(context.pBmFontStrip->Width() / strlen(context.pCharset) * strlen(_pszText), context.pBmFontStrip->Height())), pParentElmt, 0, &context)) {
        delete pFloaterElmt;
        return nullptr;     // Failed to create FloaterElmt
    }
    pFloaterElmt->SetText(_pszText);
    pFloaterElmt->LinkLastTo(listFloater);

    pFloaterElmt->StartSlider(_ptElmt-CPoint(0,100),listSlider,1);

    return pFloaterElmt;
}

// .DestroyFloaters
//
void FloaterChain::DestroyFloaters()
{
    while (FirstFloaterElmt()) {
        FirstFloaterElmt()->DestroyElement();
        delete FirstFloaterElmt();
    }
}

// .MakeFloatersTopmost
//
bool FloaterChain::MakeFloatersTopmost()
{
    for (FloaterElmt *pElmt = FirstFloaterElmt(); pElmt; pElmt = pElmt->NextFloaterElmt())
        pElmt->MoveOrderTop();

    return FirstFloaterElmt() != nullptr;
}

void FloaterChain::SlideFloaters()
{
    Slider *pSliderNext;

    for (Slider *pSlider = FirstSlider(); pSlider != nullptr; pSlider = pSliderNext) {
        pSliderNext = pSlider->NextSlider();

        if (!pSlider->StepSlider()) {
            pSlider->UnlinkSlide();
           ((FloaterElmt*)pSlider)->DestroyElement();
           delete (FloaterElmt*)pSlider;
          }
    }
}
