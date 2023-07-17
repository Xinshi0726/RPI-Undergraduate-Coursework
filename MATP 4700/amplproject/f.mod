#
# assign crops to fields
#
# maximize profit (=expected revenue - costs)
#
# each crop/field combination has an expected revenue (after accounting
#           for costs such as seed)
#
# have options to purchase additional technologies to improve yield.
#
# constraints: capacities of fields, diversity in crops, limited availability
#   of technologies
#

set CROPS;  #  crops
set FIELDS;  #  fields
set TECHNOLOGIES;  #  productivity enhancements
set TECHlimited;  #  techs limited to half the fields
set FIELDSa within FIELDS;
set FIELDSb within FIELDS;

#
# parameters
#

param yield{CROPS,FIELDS} >= 0;  # expected revenue from filling the field with the crop

param availability{TECHNOLOGIES} >= 0;  # maximum availability of each technology

param techfieldcost{FIELDS,TECHNOLOGIES} >= 0;  # cost to cover a field using a technology

param improve{CROPS,TECHNOLOGIES} >= 0;   # proportional boost in expected revenue
                                          # from using the technology
                                          # (assume benefits from different
                                          # technologies are additive)

param propUB{CROPS} >= 0 default 0.5;  # maximum proportion of expected yield allocated to CROP
param propLB{CROPS} >= 0 default 0.1;  # minimum proportion of expected yield allocated to CROP

#
# variables
#

var grow{CROPS,FIELDS} binary;  # proportion of field j taken by crop i.

var usetech{FIELDS,TECHNOLOGIES} binary;  # proportion of field j in crop i with technology k
 
var techsum{TECHNOLOGIES} >= 0; # total usage of technologies (cost)

var revenue{CROPS} >= 0;  # total amount of revenue due to CROP
                          # (this is subject to propUB and propLB,
                          # and depends on grow and usetech)

var totalrevenue >= 0;  # sum of revenue variables

var techcosts >= 0;  # sum of costs of employing technologies

var sumfielda binary;

var sumfieldb binary;

var bulkwatera >= 0;

var bulkwaterb >= 0;

var halfwatera binary;

var halfwaterb binary;

var totalwater binary;

var indifa binary;

var indifb binary;

var totalcost >= 0;

#
# objective function: minimize totalrevenue - techcosts
#

maximize profits: totalrevenue - techcosts - bulkwatera*indifa - bulkwaterb*indifb - halfwatera * 402 - halfwaterb * 402 - totalwater*800;
  
#
# constraints
#
subject to assignfield{j in FIELDS}:  #  assign crops to fields
  sum{i in CROPS} grow[i,j] = 1;

subject to assigntech{j in FIELDS}:  #  assign technologies to crops
  sum{k in TECHNOLOGIES} usetech[j,k] = 1;
  
subject to fielda{k in TECHlimited}:
  100*sumfielda - sum{j in FIELDSa} usetech[j,k] >= 0;
  
subject to fieldb{k in TECHlimited}:
  100*sumfieldb - sum{j in FIELDSb} usetech[j,k] >= 0;
  
subject to split: sumfielda + sumfieldb <= 1;

subject to onlywatera:
  bulkwatera = sum{j in FIELDSa} (techfieldcost[j,'water']*usetech[j,'water']);
    
subject to onlywaterb:
  bulkwaterb = sum{j in FIELDSb} (techfieldcost[j,'water']*usetech[j,'water']);
    
subject to india:
	indifa + sum{j in FIELDSa} usetech[j,'water'] <= 10;
	
subject to indib:
	indifb + sum{j in FIELDSb} usetech[j,'water'] <= 10;
    
subject to halfa:
  sum{j in FIELDSa} usetech[j,'water'] >= 10*halfwatera;
  
subject to halfb:
  sum{j in FIELDSb} usetech[j,'water'] >= 10*halfwaterb;
  
subject to total:
  sum{j in FIELDS} usetech[j,'water'] >= 20*totalwater;
  
subject to waterfielda:
  bulkwatera + halfwatera + totalwater = 1;

subject to waterfieldb:
  bulkwaterb + halfwaterb + totalwater = 1;
  
subject to watertwohalf:
  halfwatera + halfwaterb = 1;

subject to totaltech{k in TECHNOLOGIES: k <> 'water'}: # calculate amount spent on each technology
  sum{j in FIELDS} techfieldcost[j,k]*usetech[j,k]
    = techsum[k];
    
subject to techavail{k in TECHNOLOGIES}: techsum[k] <= availability[k];
    
subject to findcosts: techcosts = (sum{k in TECHNOLOGIES: k<> 'water'} techsum[k]);

subject to findrevenue{i in CROPS}:
  sum{j in FIELDS} yield[i,j] * ( grow[i,j] + sum{k in TECHNOLOGIES} improve[i,k]*usetech[j,k] )
    = revenue[i];
    
subject to sumrevenue: totalrevenue = sum{i in CROPS} revenue[i];

subject to respectLB{i in CROPS}: revenue[i] >= propLB[i]*totalrevenue;

subject to respectUB{i in CROPS}: revenue[i] <= propUB[i]*totalrevenue;