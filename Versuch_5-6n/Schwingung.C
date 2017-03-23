// ****** ROOT Macro to simulate forced oscillation ******

#include <TCanvas.h>
#include <TGraph.h>
#include <TMath.h>
#include <cmath>
#include <vector>
#include <sstream>
#include <string>

// --- ROOT stuff ----

TCanvas *c1 = new TCanvas("c1", "", 1200, 600);
TGraph *gr_arr[5]; 
TGraph *phase_arr[5];
TMultiGraph *gr_mg = new TMultiGraph();
TMultiGraph *phase_mg = new TMultiGraph();
TLegend *gr_leg = new TLegend(0.65, 0.65, 0.85, 0.85);
TLegend *phase_leg = new TLegend(0.65, 0.15, 0.85, 0.35);

void Schwingung(){
// --- some parameters ---

  Double_t beta = 0.05;		// dampening
  Double_t omega_0 = 2;	// eigenfrequency
  Int_t n = 20;
  
// --- some variables ---

  Double_t omega_ext = 0.;
  Double_t omega_rel;		// =omega_ext/omega_0
  Double_t phi_norm;		// normalized amplitude phi(omega)/phi(0)
  Double_t alpha;			// phase shift
  Double_t num, denum;
  ostringstream gr_name;
    
// --- now, let's calculate the amplitude --- 
  for(int k=0; k<4;k++){
	gr_name.str("");
	gr_arr[k] = new TGraph(n);
	phase_arr[k] = new TGraph(n);
	
	switch (k){
	  case 0: beta = 0.2; gr_arr[k]->SetLineColor(2); phase_arr[k]->SetLineColor(2); break;
	  case 1: beta = 0.4; gr_arr[k]->SetLineColor(3); phase_arr[k]->SetLineColor(3); break;
	  case 2: beta = 0.8; gr_arr[k]->SetLineColor(4); phase_arr[k]->SetLineColor(4); break;
	  case 3: beta = 1.8; gr_arr[k]->SetLineColor(6); phase_arr[k]->SetLineColor(6); break;
	}
	
	omega_ext=0;
    for(int i=0; i<n; i++){
	    omega_ext = (i+1)*(2*omega_0/n);//+= 2*omega_0/n;
	    omega_rel = omega_ext/omega_0;
	  
	    num = 5;//TMath::Power(omega_0,2);
	    denum = TMath::Sqrt(TMath::Power((TMath::Power(omega_0,2)-TMath::Power(omega_ext,2)),2) + (4*TMath::Power(beta,2)*TMath::Power(omega_ext,2)));
	    phi_norm =num/denum;
		
		if(omega_ext<omega_0)
			alpha = TMath::ATan(2*beta*omega_ext/(TMath::Power(omega_0,2)-TMath::Power(omega_ext,2)));
		else if(omega_ext>omega_0)
			alpha = TMath::ATan(2*beta*omega_ext/(TMath::Power(omega_0,2)-TMath::Power(omega_ext,2)))+TMath::Pi();
		else if(omega_ext==omega_0)
			alpha = TMath::Pi()/2;
		
		gr_arr[k]->SetPoint(i, omega_rel, phi_norm);
	    phase_arr[k]->SetPoint(i, omega_rel, alpha/TMath::Pi());
		std::cout<< i<<", "<<omega_rel[i]<<", "<<phi_norm[i]<<std::endl; 
    }
	 
	gr_arr[k]->SetLineWidth(2);
	phase_arr[k]->SetLineWidth(2);
	gr_name<<"beta="<<beta;
	gr_arr[k]->SetTitle(gr_name.str().c_str());
	phase_arr[k]->SetTitle(gr_name.str().c_str());
  
    gr_mg->Add(gr_arr[k]);
	phase_mg->Add(phase_arr[k]);
	
	gr_leg->AddEntry(gr_arr[k], gr_name.str().c_str(),"l");
	phase_leg->AddEntry(phase_arr[k], gr_name.str().c_str(),"l");
  }
  c1->Divide(2,1);
  
  c1->cd(1);
  gr_mg->SetTitle("Amplitude;#omega/#omega_{0};#phi_{0}(#omega)/#phi_{0}(0)");
  gr_mg->Draw("AC");
  gr_leg->Draw();
  
  c1->cd(2);
  phase_mg->SetTitle("Phase;#omega/#omega_{0};#alpha/#pi");
  phase_mg->Draw("AC");
  phase_leg->Draw();
  return;
}
