# README.md for PaNDA-Sim Geant simulation
# Dustin Scriven 6/8/18

 /////////////////////////////////////////////////////////////////////////////
/                                                                             /
/                                   README.md                                 /
/                                   PaNDA-Sim                                 /
/                                     Geant4                                  /
/                                                                             /
 /////////////////////////////////////////////////////////////////////////////

PaNDA-Sim is a Geant4 simulation used to simulation a scintillating neutron 
detector array. The origin of this simulation is from Shuya Ota-TAMU to
simulate a segmented neutron detector made of different scintillation
materials, both liquid and solid, with PMTs and a neutron source. The
purpose of this project is to adapt this code to explore several detector array
arrangements using the substance para-terphenyl. 

This README.md is intended to inform the user how to compile and run the
simulation and to act as a reference for tuning parameters. Some refence
materials will be left here too, and possibly some data regarding para-
terpenyl properties.

In order to run this we first need to compile, and to do so we need to change
directory pointers.





                        <Some para-Terphenyl Documentation>
[1] https://www.sciencedirect.com/science/article/pii/S0168900215013832
[2] https://en.wikipedia.org/wiki/Terphenyl
[3] http://www.chemspider.com/Chemical-Structure.6848.html
[4] https://pubs.acs.org/doi/abs/10.1021/acs.cgd.6b01900
[5] http://www.cryos-beta.kharkov.ua/index.php
[6] https://arxiv.org/pdf/1305.0442.pdf;
[7] https://www.sciencedirect.com/science/article/pii/S0168900215005264

                                   Table 1
###############################################################################
Propery                         Value                 Citation
H/C                             0.778                      [5]     
Time - Fast               2.1, 3.7 ns                 [1], [5]
Time - Slow                   22.6 ns                      [1]
Birks Constant          
Density                    1.23 g/cm3                      [5]
Light Output             27,000ph/MeV                      [5] 
Attenuation length            4.73 mm                      [6]  
           
###############################################################################
Table 1: para-Terphenyl parameters
