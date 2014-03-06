// @(#)root/gpad:$Id: TCreatePrimitives.cxx,v 1.0

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TCreatePrimitives                                                    //
//                                                                      //
// Creates new primitives.                                              //
//                                                                      //
// The functions in this static class are called by TPad::ExecuteEvent  //
// to create new primitives in gPad from the TPad toolbar.              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TCanvas.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TMarker.h"
#include "TGroupButton.h"
#include "TVirtualPad.h"
#include "TCreatePrimitives.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TMath.h"
#include "KeySymbols.h"



#include <iostream>

TLatex *TCreatePrimitives::fgText = 0;
TCurlyLine *TCreatePrimitives::fgCLine = 0;
TArrow *TCreatePrimitives::fgArrow = 0;
TLine *TCreatePrimitives::fgLine = 0;
TCurlyArc *TCreatePrimitives::fgCArc = 0;
TArc *TCreatePrimitives::fgArc = 0;
TEllipse *TCreatePrimitives::fgEllipse = 0;
TPave *TCreatePrimitives::fgPave = 0;
TPaveText *TCreatePrimitives::fgPaveText = 0;
TPavesText *TCreatePrimitives::fgPavesText = 0;
TDiamond *TCreatePrimitives::fgDiamond = 0;
TPaveLabel *TCreatePrimitives::fgPaveLabel = 0;
TGraph *TCreatePrimitives::fgPolyLine = 0;
TBox *TCreatePrimitives::fgPadBBox = 0;

//______________________________________________________________________________
TCreatePrimitives::TCreatePrimitives()
{
   // TCreatePrimitives default constructor
}


//______________________________________________________________________________
TCreatePrimitives::~TCreatePrimitives()
{
   // TCreatePrimitives destructor
}

//______________________________________________________________________________
void TCreatePrimitives::Ellipse(Int_t event, Int_t px, Int_t py, Int_t mode)
{
   //  Create a new arc/ellipse in this gPad
   //
   //  Click left button to indicate arrow starting position.
   //  Release left button to terminate the arrow.
   //

   static Double_t x0, y0, x1, y1;
   static Int_t px0, py0;
   Double_t xc,yc,r1,r2,xold,yold;

   switch (event) {

   case kButton1Down:
      x0 = gPad->AbsPixeltoX(px);
      y0 = gPad->AbsPixeltoY(py);
      px0   = px; py0   = py;
      xold = gPad->AbsPixeltoX(px);
      yold = gPad->AbsPixeltoY(py);
      break;

   case kButton1Motion:
      xold = gPad->AbsPixeltoX(px);
      yold = gPad->AbsPixeltoY(py);

      if (gPad->GetLogx()) xold = TMath::Power(10,xold);
      if (gPad->GetLogy()) yold = TMath::Power(10,yold);

      xc = 0.5*(x0+xold);
      yc = 0.5*(y0+yold);
      if (mode == kArc) {
         r1 = 0.5*TMath::Abs(xold-x0);
         if(fgArc) {
            fgArc->SetR1(r1);
            fgArc->SetR2(r1);
            fgArc->SetX1(xc);
            fgArc->SetY1(yc);
         }
         else {
            fgArc = new TArc(xc, yc, r1);
            fgArc->Draw();
         }
         gPad->Modified(kTRUE);
         gPad->Update();
      }
      if (mode == kEllipse) {
         r1 = 0.5*TMath::Abs(xold-x0);
         r2 = 0.5*TMath::Abs(yold-y0);
         if(fgEllipse) {
            fgEllipse->SetR1(r1);
            fgEllipse->SetR2(r2);
            fgEllipse->SetX1(xc);
            fgEllipse->SetY1(yc);
         }
         else {
            fgEllipse = new TEllipse(xc, yc, r1, r2);
            fgEllipse->Draw();
         }
         gPad->Modified(kTRUE);
         gPad->Update();
      }
      break;

   case kButton1Up:
      x1 = gPad->AbsPixeltoX(px);
      y1 = gPad->AbsPixeltoY(py);
      if (gPad->GetLogx()) {
         x0 = TMath::Power(10,x0);
         x1 = TMath::Power(10,x1);
      }
      if (gPad->GetLogy()) {
         y0 = TMath::Power(10,y0);
         y1 = TMath::Power(10,y1);
      }
      xc = 0.5*(x0+x1);
      yc = 0.5*(y0+y1);
      if (mode == kArc) fgArc = 0;
      if (mode == kEllipse) fgEllipse = 0;

      gROOT->SetEditorMode();
      break;
   }
}


//______________________________________________________________________________
void TCreatePrimitives::Line(Int_t event, Int_t px, Int_t py, Int_t mode)
{
   // Create a new line/arrow in this gPad
   //
   //  Click left button to indicate arrow starting position.
   //  Release left button to terminate the arrow.
   //

   static Double_t x0, y0;

   static Int_t pxold, pyold;
   static Int_t px0, py0;
   Double_t radius, phimin,phimax;

   switch (event) {

   case kButton1Down:
      x0 = gPad->AbsPixeltoX(px);
      y0 = gPad->AbsPixeltoY(py);
      px0   = px; py0   = py;
      pxold = px; pyold = py;
      if (gPad->GetLogx()) {
         px0 = TMath::Power(10,px0);
         pxold = TMath::Power(10,pxold);
      }
      if (gPad->GetLogy()) {
         py0 = TMath::Power(10,py0);
         pyold = TMath::Power(10,pyold);
      }
      break;

   case kButton1Motion:
      pxold = px;
      pyold = py;

      if (gPad->GetLogx()) pxold = TMath::Power(10,pxold);
      if (gPad->GetLogy()) pyold = TMath::Power(10,pyold);

      if (mode == kLine) {
         if(fgLine){
            fgLine->SetX2(gPad->AbsPixeltoX(pxold));
            fgLine->SetY2(gPad->AbsPixeltoY(pyold));
         }
         else {
            fgLine = new TLine(x0,y0,gPad->AbsPixeltoX(pxold),gPad->AbsPixeltoY(pyold));
            fgLine->Draw();
         }
         gPad->Modified(kTRUE);
         gPad->Update();
      }

      if (mode == kArrow) {
         if(fgArrow){
            fgArrow->SetX2(gPad->AbsPixeltoX(pxold));
            fgArrow->SetY2(gPad->AbsPixeltoY(pyold));
         }
         else {
            fgArrow = new TArrow(x0,y0,gPad->AbsPixeltoX(pxold),gPad->AbsPixeltoY(pyold)
                                 , TArrow::GetDefaultArrowSize()
                                 , TArrow::GetDefaultOption());
            fgArrow->Draw();
         }
         gPad->Modified(kTRUE);
         gPad->Update();
      }

      if (mode == kCurlyLine) {
         if(fgCLine) fgCLine->SetEndPoint(gPad->AbsPixeltoX(pxold), gPad->AbsPixeltoY(pyold));
         else  {
            fgCLine = new TCurlyLine(gPad->AbsPixeltoX(px0),gPad->AbsPixeltoY(py0),gPad->AbsPixeltoX(pxold),gPad->AbsPixeltoY(pyold)
                                     , TCurlyLine::GetDefaultWaveLength()
                                     , TCurlyLine::GetDefaultAmplitude());
            fgCLine->Draw();
         }
         gPad->Modified(kTRUE);
         gPad->Update();
      }

      if (mode == kCurlyArc) {
         //calculate radius in pixels and convert to users x
         radius = gPad->PixeltoX((Int_t)(TMath::Sqrt((Double_t)((px-px0)*(px-px0) + (py-py0)*(py-py0)))))
                 - gPad->PixeltoX(0);
         if(fgCArc) {
            fgCArc->SetStartPoint(gPad->AbsPixeltoX(pxold), gPad->AbsPixeltoY(pyold));
            fgCArc->SetRadius(radius);
         }
         else {
            phimin = 0;
            phimax = 360;
            fgCArc = new TCurlyArc(x0,y0,radius,phimin,phimax
                                   , TCurlyArc::GetDefaultWaveLength()
                                   , TCurlyArc::GetDefaultAmplitude());
            fgCArc->Draw();
         }
         gPad->Modified(kTRUE);
         gPad->Update();
      }
      break;

   case kButton1Up:
      if (mode == kLine) fgLine = 0;
      if (mode == kArrow) fgArrow = 0;
      if (mode == kCurlyLine) fgCLine = 0;
      if (mode == kCurlyArc) fgCArc = 0;
      gROOT->SetEditorMode();
      break;
   }
}


//______________________________________________________________________________
void TCreatePrimitives::Pad(Int_t event, Int_t px, Int_t py, Int_t)
{
   // Create a new pad in gPad
   //
   //  Click left button to indicate one corner of the pad
   //  Click left button to indicate the opposite corner
   //
   //  The new pad is inserted in the pad where the first point is selected.
   //

   static Int_t px1old, py1old, px2old, py2old;
   static Int_t px1, py1, px2, py2, pxl, pyl, pxt, pyt;
   static TPad *padsav;
   Double_t xlow, ylow, xup, yup;
   TPad * newpad;

   Int_t  n = 0;
   TObject *obj;
   TIter next(gPad->GetListOfPrimitives());

   while ((obj = next())) {
      if (obj->InheritsFrom(TPad::Class())) {
         n++;
      }
   }

   switch (event) {

   case kButton1Down:
      padsav = (TPad*)gPad;
      gPad->cd();
      px1 = gPad->XtoAbsPixel(gPad->GetX1());
      py1 = gPad->YtoAbsPixel(gPad->GetY1());
      px2 = gPad->XtoAbsPixel(gPad->GetX2());
      py2 = gPad->YtoAbsPixel(gPad->GetY2());
      px1old = px; py1old = py;
      break;

   case kButton1Motion:
      px2old = px;
      px2old = TMath::Max(px2old, px1);
      px2old = TMath::Min(px2old, px2);
      py2old = py;
      py2old = TMath::Max(py2old, py2);
      py2old = TMath::Min(py2old, py1);
      pxl = TMath::Min(px1old, px2old);
      pxt = TMath::Max(px1old, px2old);
      pyl = TMath::Max(py1old, py2old);
      pyt = TMath::Min(py1old, py2old);

      if(fgPadBBox) {
         fgPadBBox->SetX1(gPad->AbsPixeltoX(pxl));
         fgPadBBox->SetY1(gPad->AbsPixeltoY(pyl));
         fgPadBBox->SetX2(gPad->AbsPixeltoX(pxt));
         fgPadBBox->SetY2(gPad->AbsPixeltoY(pyt));
      }
      else {
         fgPadBBox = new TBox(pxl, pyl, pxt, pyt);
         fgPadBBox->Draw("l");
      }
      gPad->Modified(kTRUE);
      gPad->Update();
      break;

   case kButton1Up:
      fgPadBBox->Delete();
      fgPadBBox = 0;
      xlow = (Double_t(pxl) - Double_t(px1))/(Double_t(px2) - Double_t(px1));
      ylow = (Double_t(py1) - Double_t(pyl))/(Double_t(py1) - Double_t(py2));
      xup  = (Double_t(pxt) - Double_t(px1))/(Double_t(px2) - Double_t(px1));
      yup  = (Double_t(py1) - Double_t(pyt))/(Double_t(py1) - Double_t(py2));
      gROOT->SetEditorMode();
      if (xup <= xlow || yup <= ylow) return;
      newpad = new TPad(Form("%s_%d",gPad->GetName(),n+1),"newpad",xlow, ylow, xup, yup);
      if (newpad->IsZombie()) break;
      newpad->SetFillColor(gStyle->GetPadColor());
      newpad->Draw();
      TCanvas *canvas = gPad->GetCanvas();
      if (canvas) canvas->Selected((TPad*)gPad, newpad, event);
      padsav->cd();
      break;
   }
}


//______________________________________________________________________________
void TCreatePrimitives::Pave(Int_t event, Int_t px, Int_t py, Int_t mode)
{
   // Create a new pavetext in gPad
   //
   //  Click left button to indicate one corner of the pavelabel.
   //  Release left button at the opposite corner.
   //

   static Double_t x0, y0;

   static Int_t pxold, pyold;
   static Int_t px0, py0;
   Double_t xp0,xp1,yp0,yp1,xold,yold;

   if (mode == kPaveLabel)
      ((TPad *)gPad)->EventPave();

   switch (event) {

   case kKeyPress:
      if (mode == kPaveLabel) {
         if ((py == kKey_Return) || (py == kKey_Enter)) {
            TString s(fgPaveLabel->GetTitle());
            Int_t l = s.Length();
            s.Remove(l-1);
            fgPaveLabel->SetLabel(s.Data());
            gSystem->ProcessEvents();
            gPad->Modified(kTRUE);
            gROOT->SetEditorMode();
            gPad->Update();
            fgPaveLabel = 0;
         } else if (py == kKey_Backspace) {
            TString s(fgPaveLabel->GetTitle());
            Int_t l = s.Length();
            if (l>1) {
               s.Replace(l-2, 2, "<");
               fgPaveLabel->SetLabel(s.Data());
               gPad->Modified(kTRUE);
               gPad->Update();
            }
         } else if (isprint(py)) {
            TString s(fgPaveLabel->GetTitle());
            Int_t l = s.Length();
            s.Insert(l-1,(char)py);
            fgPaveLabel->SetLabel(s.Data());
            gPad->Modified(kTRUE);
            gPad->Update();
         }
      }
      break;

   case kButton1Down:
      x0 = gPad->AbsPixeltoX(px);
      y0 = gPad->AbsPixeltoY(py);
      px0   = px; py0   = py;
      pxold = px; pyold = py;
      break;

   case kButton1Motion:
      pxold = px;
      pyold = py;

      xold = gPad->AbsPixeltoX(px);
      yold = gPad->AbsPixeltoY(py);

      xp0 = gPad->PadtoX(x0);
      xp1 = gPad->PadtoX(xold);
      yp0 = gPad->PadtoY(y0);
      yp1 = gPad->PadtoY(yold);

      if (mode == kPave) {
         if(fgPave) {
            if(xold < x0) {
               fgPave->SetX1(xold);
               fgPave->SetX2(x0);
            }
            else {
               fgPave->SetX1(x0);
               fgPave->SetX2(xold);
            }
            if(yold < y0) {
               fgPave->SetY1(yold);
               fgPave->SetY2(y0);
            }
            else {
               fgPave->SetY1(y0);
               fgPave->SetY2(yold);
            }
         }
         else {
            fgPave = new TPave(xp0,yp0,xp1,yp1);
            fgPave->Draw();
         }
         gPad->Modified(kTRUE);
         gPad->Update();
      }

      if (mode == kPaveText ) {
         if(fgPaveText){
            if(xold < x0) {
               fgPaveText->SetX1(xold);
               fgPaveText->SetX2(x0);
            }
            else {
               fgPaveText->SetX1(x0);
               fgPaveText->SetX2(xold);
            }
            if(yold < y0) {
               fgPaveText->SetY1(yold);
               fgPaveText->SetY2(y0);
            }
            else {
               fgPaveText->SetY1(y0);
               fgPaveText->SetY2(yold);
            }
         }
         else {
            fgPaveText = new TPaveText(xp0,yp0,xp1,yp1);
            fgPaveText->Draw();
         }
         gPad->Modified(kTRUE);
         gPad->Update();
      }

      if (mode == kPavesText) {
         if(fgPavesText){
            if(xold < x0) {
               fgPavesText->SetX1(xold);
               fgPavesText->SetX2(x0);
            }
            else {
               fgPavesText->SetX1(x0);
               fgPavesText->SetX2(xold);
            }
            if(yold < y0) {
               fgPavesText->SetY1(yold);
               fgPavesText->SetY2(y0);
            }
            else {
               fgPavesText->SetY1(y0);
               fgPavesText->SetY2(yold);
            }
         }
         else {
            fgPavesText = new TPavesText(xp0,yp0,xp1,yp1);
            fgPavesText->Draw();
         }
         gPad->Modified(kTRUE);
         gPad->Update();
      }

      if (mode == kPaveLabel) {
         if(fgPaveLabel){
            if(xold < x0) {
               fgPaveLabel->SetX1(xold);
               fgPaveLabel->SetX2(x0);
            }
            else {
               fgPaveLabel->SetX1(x0);
               fgPaveLabel->SetX2(xold);
            }
            if(yold < y0) {
               fgPaveLabel->SetY1(yold);
               fgPaveLabel->SetY2(y0);
            }
            else {
               fgPaveLabel->SetY1(y0);
               fgPaveLabel->SetY2(yold);
            }
         }
         else {
            fgPaveLabel = new TPaveLabel(xp0,yp0,xp1,yp1,">");
            fgPaveLabel->Draw();
         }
         gPad->Modified(kTRUE);
         gPad->Update();
      }

      if (mode == kDiamond) {
         if(fgDiamond){
            fgDiamond->SetX1(x0);
            fgDiamond->SetY1(y0);
            fgDiamond->SetX2(xold);
            fgDiamond->SetY2(yold);
         }
         else {
            fgDiamond = new TDiamond(x0,y0,xold,yold);
            fgDiamond->Draw();
         }
         gPad->Modified(kTRUE);
         gPad->Update();
      }
      break;

   case kButton1Up:
      if (mode == kPave)      fgPave = 0;
      if (mode == kPaveText ) fgPaveText = 0;
      if (mode == kPavesText) fgPavesText = 0;
      if (mode == kDiamond)   fgDiamond = 0;
      if (mode == kPaveLabel) {
         ((TPad *)gPad)->StartEditing();
         gSystem->ProcessEvents();
         if (mode == kPaveLabel) {
            gPad->Modified(kTRUE);
            gPad->Update();
            break;
         }
      }
      gROOT->SetEditorMode();
      break;
   }
}

//______________________________________________________________________________
void TCreatePrimitives::PolyLine(Int_t event, Int_t px, Int_t py, Int_t mode)
{
   // Create a new PolyLine in gPad
   //
   //  Click left button to indicate a new point
   //  Click left button at same place or double click to close the polyline
   //

   static Int_t pxnew, pynew;
   Double_t xnew, ynew, xold, yold;
   static Int_t npoints = 0;
   Double_t dp;

   switch (event) {

   case kButton1Down:
      if(npoints > 0) {
         pxnew = px;
         pynew = py;
      }
      else
      {
         pxnew = px;
         pynew = py;
      }
      npoints++;
      if(fgPolyLine) {
         fgPolyLine->Set(fgPolyLine->GetN() +1);
         fgPolyLine->SetPoint(npoints, gPad->PadtoX(gPad->AbsPixeltoX(pxnew)),
                gPad->PadtoY(gPad->AbsPixeltoY(pynew)));
         // stop collecting new points if new point is close ( < 5 pixels) of previous point
         if(npoints > 1) {
            fgPolyLine->GetPoint(fgPolyLine->GetN()-3, xold, yold);
            xnew = gPad->PadtoX(gPad->AbsPixeltoX(pxnew));
            ynew = gPad->PadtoY(gPad->AbsPixeltoY(pynew));
            dp = TMath::Abs(xnew-xold) +TMath::Abs(ynew-yold);
            if (dp < 0.007) {
               if(mode == kPolyLine) {
                  fgPolyLine->SetPoint(npoints, gPad->PadtoX(gPad->AbsPixeltoX(pxnew)),
                                  gPad->PadtoY(gPad->AbsPixeltoY(pynew)));
               }
               else {
                  fgPolyLine->GetPoint(0, xnew, ynew);
                  fgPolyLine->SetPoint(npoints, xnew, ynew);
               }
               fgPolyLine = 0;
               npoints = 0;
               gPad->Modified();
               gPad->Update();
               gROOT->SetEditorMode();
            }
         }
      }
      break;

   case kButton1Double:
      if(fgPolyLine) {
         if(mode == kPolyLine) {
            fgPolyLine->SetPoint(npoints, gPad->PadtoX(gPad->AbsPixeltoX(pxnew)),
                            gPad->PadtoY(gPad->AbsPixeltoY(pynew)));
         }
         else {
            fgPolyLine->GetPoint(0, xnew, ynew);
            fgPolyLine->SetPoint(npoints, xnew, ynew);
         }
         fgPolyLine = 0;
         npoints = 0;
         gPad->Modified();
         gPad->Update();
         gROOT->SetEditorMode();
      }
      break;

   case kMouseMotion:
      pxnew = px;
      pynew = py;
      if(fgPolyLine){
         fgPolyLine->SetPoint(npoints, gPad->PadtoX(gPad->AbsPixeltoX(pxnew)),
                         gPad->PadtoY(gPad->AbsPixeltoY(pynew)));
         gPad->Modified();
         gPad->Update();
      }
      else {
         if (mode == kPolyLine) {
            fgPolyLine = new TGraph(1);
            fgPolyLine->ResetBit(TGraph::kClipFrame);
         } else {
            fgPolyLine = (TGraph*)gROOT->ProcessLineFast(
                 Form("new %s(\"CUTG\",%d",
                      gROOT->GetCutClassName(),1));
         }
         fgPolyLine->SetPoint(0, gPad->PadtoX(gPad->AbsPixeltoX(pxnew)),
                         gPad->PadtoY(gPad->AbsPixeltoY(pynew)));
         fgPolyLine->Draw("L");
      }
      break;
   }
}

//______________________________________________________________________________
void TCreatePrimitives::Text(Int_t event, Int_t px, Int_t py, Int_t mode)
{
   // Create a new TLatex at the cursor position in gPad
   //
   // Click left button to indicate the text position
   //

   static Double_t x, y;

   switch (event) {

   case kKeyPress:
      if ((py == kKey_Return) || (py == kKey_Enter)) {
         TString s(fgText->GetTitle());
         Int_t l = s.Length();
         s.Remove(l-1);
         fgText->SetText(x,y,s.Data());
         gSystem->ProcessEvents();
         gPad->Modified(kTRUE);
         gROOT->SetEditorMode();
         gPad->Update();
         fgText = 0;
      } else if (py == kKey_Backspace) {
         TString s(fgText->GetTitle());
         Int_t l = s.Length();
         if (l>1) {
            s.Replace(l-2, 2, "<");
            fgText->SetText(x,y,s.Data());
            gPad->Modified(kTRUE);
            gPad->Update();
         }
      } else if (isprint(py)) {
         TString s(fgText->GetTitle());
         Int_t l = s.Length();
         s.Insert(l-1,(char)py);
         fgText->SetText(x,y,s.Data());
         gPad->Modified(kTRUE);
         gPad->Update();
      }
      break;

   case kButton1Down:
     if (fgText) {
         TString s(fgText->GetTitle());
         Int_t l = s.Length();
         s.Remove(l-1);
         fgText->SetText(x,y,s.Data());
      }

      x = gPad->AbsPixeltoX(px);
      y = gPad->AbsPixeltoY(py);
      if (gPad->GetLogx()) x = TMath::Power(10,x);
      if (gPad->GetLogy()) y = TMath::Power(10,y);

      if (mode == kMarker) {
         TMarker *marker;
         marker = new TMarker(x,y,gStyle->GetMarkerStyle());
         marker->Draw();
         gROOT->SetEditorMode();
         break;
      }

      ((TPad *)gPad)->StartEditing();
      gSystem->ProcessEvents();

      fgText = new TLatex(x,y,"<");
      fgText->Draw();
      gPad->Modified(kTRUE);
      gPad->Update();
      break;
   }
}
