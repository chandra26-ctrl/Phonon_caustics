//This is a root Program to Plot the Caustic Patterns from the phonon_hits.txt File
//Function to Plot Only Phonon Caustic.

const int nBinsX = 300;
const int nBinsY = 300;
const double minX = -4;
const double minY = -4;
const double maxX = 4;
const double maxY = 4;

const double meterToMillimeter = 1000.0;

//---------------------------------------------------------------------------------
//Only plot transverse fast phonons
void TransFast(const TString& fileName) {

  //Get the file output from the example
  ifstream in;
  in.open(fileName);
  TH2D *Caustics= new TH2D("Caustics","Phonon Caustics;X position [mm];Y position [mm]",nBinsX,minX,maxX,nBinsY,minY,maxY);
  Int_t nlines = 0;
  Int_t EventID=-1, TrackID=-1;
  TString Name_Phonon;
  Double_t X_f,Y_f,Z_f;

  //Read in the phonons from the file
  while (1) {

     in>>EventID>>TrackID>>Name_Phonon>>X_f>>Y_f>>Z_f;
     if(Name_Phonon=="phononTF"){
     Caustics->Fill(X_f * meterToMillimeter,Y_f * meterToMillimeter);
    }
     if (!in.good()) break;
  nlines++;
  }

  TCanvas *c1 = new TCanvas("c1","Canvas Example",200,10,600,480);
  c1->SetLeftMargin(0.15);  // Leave room for the Y-axis title
  //c1->SetFillColor(1);
  //Caustics->SetMarkerColorAlpha(kWhite, 0.2);
  c1->SetLogz();

  //Caustics->Draw("colz");
  Caustics->Draw("colz");
  in.close();

}


//---------------------------------------------------------------------------------
//Only plot transverse slow phonons
void TransSlow(const TString& fileName) {

  //Get the file output from the example
  ifstream in;
  in.open(fileName);
  TH2D *Caustics= new TH2D("Caustics","Phonon Caustics;X position [mm];Y position [mm]",nBinsX,minX,maxX,nBinsY,minY,maxY);
  Int_t nlines = 0;
  Int_t EventID=-1, TrackID=-1;
  TString Name_Phonon;
  Double_t X_f,Y_f,Z_f;
  
  //Read in the phonons from the file
  while (1) {    
    in>>EventID>>TrackID>>Name_Phonon>>X_f>>Y_f>>Z_f;
    if(Name_Phonon=="phononTS"){
      Caustics->Fill(X_f * meterToMillimeter,Y_f * meterToMillimeter);
    }
    if (!in.good()) break;
    nlines++;
  }

  
  //printf(" found %d points\n",nlines);
  //gStyle->SetPalette(kBlack+3);
  TCanvas *c1 = new TCanvas("c1","Canvas Example",200,10,600,480);
  c1->SetLeftMargin(0.15);  // Leave room for the Y-axis title
  c1->SetFillColor(kWhite);
  //Caustics->SetMarkerColorAlpha(kWhite, 0.2);
  c1->SetLogz();

  Caustics->SetMinimum(1);
  //Caustics->Draw("colz"); // If you want to see on a Blue and yellow colors
  Caustics->Draw("colz");
  in.close();

}

//---------------------------------------------------------------------------------
//Only plot Longitudinal phonons together
void Longitudinal(const TString& fileName) {

  //Get the file output from the example
  ifstream in;
  in.open(fileName);
  TH2D *Caustics= new TH2D("Caustics","Phonon Caustics;X position [mm];Y position [mm]",nBinsX,minX,maxX,nBinsY,minY,maxY);
  Int_t nlines = 0;
  Int_t EventID=-1, TrackID=-1;
  TString Name_Phonon;
  Double_t X_f,Y_f,Z_f;
  
  //Read in the phonons from the file
  while (1) {    
    in>>EventID>>TrackID>>Name_Phonon>>X_f>>Y_f>>Z_f;
    if(Name_Phonon=="phononL"){
      Caustics->Fill(X_f * meterToMillimeter,Y_f * meterToMillimeter);
    }
    if (!in.good()) break;
    nlines++;
  }

  
  //printf(" found %d points\n",nlines);
  //gStyle->SetPalette(kBlack+3);
  TCanvas *c1 = new TCanvas("c1","Canvas Example",200,10,600,480);
  c1->SetLeftMargin(0.15);  // Leave room for the Y-axis title
  c1->SetFillColor(kWhite);
  //Caustics->SetMarkerColorAlpha(kWhite, 0.2);
  c1->SetLogz();

  Caustics->SetMinimum(1);
  //Caustics->Draw("colz"); // If you want to see on a Blue and yellow colors
  Caustics->Draw("colz");
  in.close();

}

//---------------------------------------------------------------------------------
// plot all phonons together
void all(const TString& fileName) {

  //Get the file output from the example
  ifstream in;
  in.open(fileName);
  TH2D *Caustics= new TH2D("Caustics","Phonon Caustics;X position [mm];Y position [mm]",nBinsX,minX,maxX,nBinsY,minY,maxY);
  Int_t nlines = 0;
  Int_t EventID=-1, TrackID=-1;
  TString Name_Phonon;
  Double_t X_f,Y_f,Z_f;
  
  //Read in the phonons from the file
  while (1) {

     in>>EventID>>TrackID>>Name_Phonon>>X_f>>Y_f>>Z_f;

     Caustics->Fill(X_f * meterToMillimeter,Y_f * meterToMillimeter);

     if (!in.good()) break;
  nlines++;
  }
  //printf(" found %d points\n",nlines);
  //gStyle->SetPalette(kBlack+3);
  TCanvas *c1 = new TCanvas("c1","Canvas Example",200,10,600,480);
  c1->SetLeftMargin(0.15);  // Leave room for the Y-axis title
  c1->SetFillColor(kWhite);
  //Caustics->SetMarkerColorAlpha(kWhite, 0.2);
  c1->SetLogz();
  Caustics->SetMinimum(1);

  Caustics->Draw("colz");
  //Caustics->Draw();
  in.close();

}

//---------------------------------------------------------------------------------
//Main function
#include <iostream>
using namespace std;
void Caustics_Plots(TString Phonon_Name, TString fileName="phonon_hits.txt") {
  
  TString  Phonon_Case = Phonon_Name;
  Int_t What_phonon1,What_phonon2,What_phonon3,What_phonon4;
  What_phonon1=Phonon_Case.CompareTo("Fast");
  What_phonon2=Phonon_Case.CompareTo("Slow");
  What_phonon3=Phonon_Case.CompareTo("Long");
  What_phonon4=Phonon_Case.CompareTo("All");
  if (What_phonon1==0) {
    cout<<"Fast";
    TransFast(fileName);
  } else if (What_phonon2==0) {
    cout<<"Slow";
    TransSlow(fileName);
  } else if (What_phonon3==0) {
    cout<<"Long";
    Longitudinal(fileName);
  } else if (What_phonon4==0) {
    cout<<"All";
    all(fileName);
  }
}
