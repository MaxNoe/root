// @(#)root/tree:$Name:  $:$Id: TBranchElement.h,v 1.2 2001/01/16 16:24:39 brun Exp $
// Author: Rene Brun   14/01/2001

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TBranchElement
#define ROOT_TBranchElement


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TBranchElement                                                       //
//                                                                      //
// A Branch for the case of an object.                                  //
//////////////////////////////////////////////////////////////////////////


#ifndef ROOT_TBranch
#include "TBranch.h"
#endif

class TStreamerInfo;

class TBranchElement : public TBranch {

protected:
    enum { kWarn = BIT(12) };

    TString             fClassName;    //Class name of referenced object
    Int_t               fClassVersion; //Version number of class
    Int_t               fID;           //element serial number in fInfo
    Int_t               fType;         //type of data in branch
    TStreamerInfo      *fInfo;         //!Pointer to StreamerInfo
    
public:
    TBranchElement();
    TBranchElement(const char *name, TStreamerInfo *sinfo, Int_t id, Int_t basketsize=32000, Int_t splitlevel = 0, Int_t compress=-1);
    virtual ~TBranchElement();

    virtual void    Browse(TBrowser *b);
    virtual Int_t   Fill();
    virtual Int_t   GetEntry(Int_t entry=0, Int_t getall = 0);
    virtual Int_t   GetID() {return fID;}
    TStreamerInfo  *GetInfo() const {return fInfo;}
    Bool_t          IsFolder() const;
    virtual void    Print(Option_t *option="") const;
    virtual void    Reset(Option_t *option="");
    virtual void    SetAddress(void *addobj);
    virtual void    SetAutoDelete(Bool_t autodel=kTRUE);
    virtual void    SetBasketSize(Int_t buffsize);
    virtual void    UpdateAddress();

    ClassDef(TBranchElement,1)  //Branch in case of an object
};

#endif
