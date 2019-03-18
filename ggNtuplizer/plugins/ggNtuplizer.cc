#include "ggAnalysis/ggNtuplizer/interface/ggNtuplizer.h"

using namespace std;
using namespace edm;

void ggNtuplizer::endJob() {
  tree_->BuildIndex("run", "event");
  std::cout<<"TTree "<<tree_->GetName()<<" indexed on run and event."<<std::endl<<" Complete!"<<std::endl;
};



ggNtuplizer::ggNtuplizer(const edm::ParameterSet& ps) :
hltPrescaleProvider_(ps, consumesCollector(), *this)
{

  development_               = ps.getParameter<bool>("development");
  addFilterInfoAOD_          = ps.getParameter<bool>("addFilterInfoAOD");
  addFilterInfoMINIAOD_      = ps.getParameter<bool>("addFilterInfoMINIAOD");
  doNoHFMET_                 = ps.getParameter<bool>("doNoHFMET");

  doGenParticles_            = ps.getParameter<bool>("doGenParticles");
  runOnParticleGun_          = ps.getParameter<bool>("runOnParticleGun");
  runOnSherpa_               = ps.getParameter<bool>("runOnSherpa");
  dumpPFPhotons_             = ps.getParameter<bool>("dumpPFPhotons");
  dumpJets_                  = ps.getParameter<bool>("dumpJets");
  dumpAK8Jets_               = ps.getParameter<bool>("dumpAK8Jets");
  dumpSoftDrop_              = ps.getParameter<bool>("dumpSoftDrop");
  dumpTaus_                  = ps.getParameter<bool>("dumpTaus");
  dumpPDFSystWeight_         = ps.getParameter<bool>("dumpPDFSystWeight");
  dumpHFElectrons_           = ps.getParameter<bool>("dumpHFElectrons");
  year_                      = ps.getParameter<int>("year");

  trgFilterDeltaPtCut_       = ps.getParameter<double>("trgFilterDeltaPtCut");
  trgFilterDeltaRCut_        = ps.getParameter<double>("trgFilterDeltaRCut");

  vtxLabel_                  = consumes<reco::VertexCollection>        (ps.getParameter<InputTag>("VtxLabel"));
  vtxBSLabel_                = consumes<reco::VertexCollection>        (ps.getParameter<InputTag>("VtxBSLabel"));
  rhoLabel_                  = consumes<double>                        (ps.getParameter<InputTag>("rhoLabel"));
  rhoCentralLabel_           = consumes<double>                        (ps.getParameter<InputTag>("rhoCentralLabel"));
  trgEventLabel_             = consumes<trigger::TriggerEvent>         (ps.getParameter<InputTag>("triggerEvent"));
  triggerObjectsLabel_       = consumes<pat::TriggerObjectStandAloneCollection>(ps.getParameter<edm::InputTag>("triggerEvent"));
  trgResultsLabel_           = consumes<edm::TriggerResults>           (ps.getParameter<InputTag>("triggerResults"));
  patTrgResultsLabel_        = consumes<edm::TriggerResults>           (ps.getParameter<InputTag>("patTriggerResults"));
  trgResultsProcess_         =                                          ps.getParameter<InputTag>("triggerResults").process();
  generatorLabel_            = consumes<GenEventInfoProduct>           (ps.getParameter<InputTag>("generatorLabel"));
  lheEventLabel_             = consumes<LHEEventProduct>               (ps.getParameter<InputTag>("LHEEventLabel"));
  puCollection_              = consumes<vector<PileupSummaryInfo> >    (ps.getParameter<InputTag>("pileupCollection"));
  genParticlesCollection_    = consumes<vector<reco::GenParticle> >    (ps.getParameter<InputTag>("genParticleSrc"));
  pfMETlabel_                = consumes<View<pat::MET> >               (ps.getParameter<InputTag>("pfMETLabel"));
  electronCollection_        = consumes<View<pat::Electron> >          (ps.getParameter<InputTag>("electronSrc"));
  gsfTracks_                 = consumes<View<reco::GsfTrack>>          (ps.getParameter<InputTag>("gsfTrackSrc"));

  BadChCandFilterToken_      = consumes<bool>                          (ps.getParameter<InputTag>("BadChargedCandidateFilter"));
  BadPFMuonFilterToken_      = consumes<bool>                          (ps.getParameter<edm::InputTag>("BadPFMuonFilter"));

  photonCollection_          = consumes<View<pat::Photon> >            (ps.getParameter<InputTag>("photonSrc"));
  muonCollection_            = consumes<View<pat::Muon> >              (ps.getParameter<InputTag>("muonSrc"));
  ebReducedRecHitCollection_ = consumes<EcalRecHitCollection>          (ps.getParameter<InputTag>("ebReducedRecHitCollection"));
  eeReducedRecHitCollection_ = consumes<EcalRecHitCollection>          (ps.getParameter<InputTag>("eeReducedRecHitCollection"));
  esReducedRecHitCollection_ = consumes<EcalRecHitCollection>          (ps.getParameter<InputTag>("esReducedRecHitCollection")); 
  recophotonCollection_      = consumes<reco::PhotonCollection>        (ps.getParameter<InputTag>("recoPhotonSrc"));
  tracklabel_                = consumes<reco::TrackCollection>         (ps.getParameter<InputTag>("TrackLabel"));
  gsfElectronlabel_          = consumes<reco::GsfElectronCollection>   (ps.getParameter<InputTag>("gsfElectronLabel"));
  tauCollection_             = consumes<vector<pat::Tau> >             (ps.getParameter<InputTag>("tauSrc"));
  pfAllParticles_            = consumes<reco::PFCandidateCollection>   (ps.getParameter<InputTag>("PFAllCandidates"));
  pckPFCandidateCollection_  = consumes<pat::PackedCandidateCollection>(ps.getParameter<InputTag>("packedPFCands"));
  pckPFCdsLabel_             = consumes<vector<pat::PackedCandidate>>  (ps.getParameter<InputTag>("packedPFCands"));
  recoCdsLabel_              = consumes<View<reco::Candidate>>         (ps.getParameter<InputTag>("packedPFCands"));

  ak4PFJetsCHSLabel_              = consumes<View<pat::Jet> >               (ps.getParameter<InputTag>("ak4PFJetsCHSSrc"));
  ak4PFJetsPUPPILabel_              = consumes<View<pat::Jet> >               (ps.getParameter<InputTag>("ak4PFJetsPUPPISrc"));
  ak8JetsPUPPILabel_              = consumes<View<pat::Jet> >               (ps.getParameter<InputTag>("ak8JetsPUPPISrc"));
  
  ak4PFJetsCHSGenJetLabel_              = consumes<std::vector<reco::GenJet> >               (ps.getParameter<InputTag>("ak4PFJetsCHSGenJetLabel"));
  //boostedDoubleSVLabel_      = consumes<reco::JetTagCollection>        (ps.getParameter<InputTag>("boostedDoubleSVLabel"));
  newparticles_              =                                          ps.getParameter< vector<int > >("newParticles");
  //jecAK8PayloadNames_        =                                          ps.getParameter<std::vector<std::string> >("jecAK8PayloadNames"); 

  //pfLooseId_                 = ps.getParameter<ParameterSet>("pfLooseId");

  // cicPhotonId_ = new CiCPhotonID(ps);

  Service<TFileService> fs;
  tree_    = fs->make<TTree>("EventTree", "Event data (tag V09_04_09_00)");
  hEvents_ = fs->make<TH1F>("hEvents",    "total processed and skimmed events",   2,  0,   1);

  branchesGlobalEvent(tree_);

  if (doGenParticles_) {
    branchesGenInfo(tree_, fs);
    branchesGenPart(tree_);
  }

  branchesMET(tree_);
  branchesPhotons(tree_);
  branchesElectrons(tree_);
  branchesMuons(tree_);
  if (dumpJets_)        branchesAK4CHSJets(tree_);
  if (dumpAK8Jets_)     branchesAK8PUPPIJets(tree_);
  if(dumpJets_ && doGenParticles_) branchesGenJetPart(tree_);
}

ggNtuplizer::~ggNtuplizer() {
}

void ggNtuplizer::analyze(const edm::Event& e, const edm::EventSetup& es) {

  hEvents_->Fill(0.2);

  // if (doGenParticles_) {
  //   jetResolution_   = JME::JetResolution::get(es, "AK4PFchs_pt");
  //   jetResolutionSF_ = JME::JetResolutionScaleFactor::get(es, "AK4PFchs");
  //   AK8jetResolution_   = JME::JetResolution::get(es, "AK8PFchs_pt");
  //   AK8jetResolutionSF_ = JME::JetResolutionScaleFactor::get(es, "AK8PFchs");
  // }

  edm::Handle<reco::VertexCollection> vtxHandle;
  e.getByToken(vtxLabel_, vtxHandle);

  reco::Vertex vtx;

  // best-known primary vertex coordinates
  math::XYZPoint pv(0, 0, 0);
  for (vector<reco::Vertex>::const_iterator v = vtxHandle->begin(); v != vtxHandle->end(); ++v) {
    // replace isFake() for miniAOD since it requires tracks while miniAOD vertices don't have tracks:
    // Vertex.h: bool isFake() const {return (chi2_==0 && ndof_==0 && tracks_.empty());}
    bool isFake = (v->chi2() == 0 && v->ndof() == 0);

    if (!isFake) {
      pv.SetXYZ(v->x(), v->y(), v->z());
      vtx = *v;
      break;
    }
  }

  initTriggerFilters(e);
  fillGlobalEvent(e, es);

  if (!e.isRealData()) {
    fillGenInfo(e);
    if (doGenParticles_)
      fillGenPart(e);
  }

  fillMET(e, es);
  fillElectrons(e, es, pv);
  fillMuons(e, pv, vtx);
  fillPhotons(e, es); 
  if (dumpJets_)         fillAK4CHSJets(e,es);
  if (dumpAK8Jets_)      fillAK8PUPPIJets(e,es);
  if(dumpJets_ && doGenParticles_) fillGenJetInfo(e);

  tree_->Fill();
  hEvents_->Fill(0.8);

}

// void ggNtuplizer::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
// {
//   //The following says we do not know what parameters are allowed so do no validation
//   // Please change this to state exactly what you do use, even if it is no parameters
//   edm::ParameterSetDescription desc;
//   desc.setUnknown();
//   descriptions.addDefault(desc);
// }

DEFINE_FWK_MODULE(ggNtuplizer);