#ifndef MDMTRACE_H
#define MDMTRACE_H

class MDMTrace {
 public:
  static MDMTrace* Instance();
  void SetBeamEnergy(double);
  double GetBeamEnergy() const;
  void SetMDMAngle(double);
  double GetMDMAngle() const;
  void SetMDMBRho(double);
  void SetMDMDipoleField(double);
  double GetMDMDipoleField() const;
  void SetScatteredAngle(double);
  void SetScatteredAngle(double,double);
  double GetScatteredAngle() const;
  void SetScatteredEnergy(double);
  double GetScatteredEnergy() const;
  void SetQValue(double);
  double GetQValue() const;
  void SetResidualEnergy(double);
  double GetResidualEnergy() const;
  void SetTargetMass(double);
  double GetTargetMass() const;
  void SetProjectileMass(double);	       
  double GetProjectileMass() const;	       
  void SetScatteredMass(double);
  double GetScatteredMass() const;
  void SetScatteredCharge(double);
  double GetScatteredCharge() const;
  double GetEnergyAfterKinematics() const;
  void SendRayWithKinematics();
  void SendRay();
  void GetPositionAngleFirstWire(double&,double&) const;
  void GetPositionAngleFirstWire(double&,double&,double&,double&) const;
  void GetOxfordWirePositions(double&,double&,double&,double&);
 private:
  MDMTrace() {};
  static MDMTrace* instance_;
  static double jeffParams_[6];
  static double oxfordWireSpacing_[3];
  double beamEnergy_;
  double scatteredEnergy_;
  double scatteredAngles_[2];
};

#endif
