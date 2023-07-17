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
#   of technologies, limited demand (could have piecewise linear revenue)
#

set CROPS;  #  crops
set FIELDS;  #  fields
set TECHNOLOGIES;  #  productivity enhancements
set TECHlimited;  #  techs limited to half the fields
set FIELDSa within FIELDS;
set FIELDSb within FIELDS;

###set NODES := PLANTS union CITIES;

###set EDGES within {NODES, NODES};  # any reversible edge is
###                                    # included twice

#
# params used in random number generation
#

#param cropbaseline{CROPS} >= 0;
#param fieldbaseline{FIELDS} >= 0;
#param techbaseline{TECHNOLOGIES} >= 0;

#
# parameters
#

param yield{CROPS,FIELDS} >= 0;  # expected revenue from filling the field with the crop
#param yield{i in CROPS,j in FIELDS} := cropbaseline[i] * fieldbaseline[j] * (0.9+ 0.2*Uniform01());

#param demand{CROPS} >= 0;  #  limit on demand

param availability{TECHNOLOGIES} >= 0;  # maximum availability of each technology

param techfieldcost{FIELDS,TECHNOLOGIES} >= 0;  # cost to cover a field using a technology
#param techfieldcost{j in FIELDS,k in TECHNOLOGIES}
#  :=fieldbaseline[j] * techbaseline[k] * (0.9+ 0.2*Uniform01());

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
var usetech{CROPS,FIELDS,TECHNOLOGIES} binary;  # proportion of field j in crop i with technology k 
var techsum{TECHNOLOGIES} >= 0; # total usage of technologies (cost)

var revenue{CROPS} >= 0;  # total amount of revenue due to CROP
                          # (this is subject to propUB and propLB,
                          # and depends on grow and usetech)

var totalrevenue >= 0;  # sum of revenue variables

var techcosts >= 0;  # sum of costs of employing technologies

var watercost >= 0;

var techbin{TECHNOLOGIES,FIELDS} binary;
#
# objective function: minimize totalrevenue - techcosts

var limited{TECHlimited} binary;

var halfbasea binary;
var halfbaseb binary;
var perbasea binary;
var perbaseb binary;
var bothbases binary;
var fieldausage{FIELDSa} >= 0;
var fieldbusage{FIELDSb} >= 0;

maximize profits: totalrevenue - techcosts - (sum {a in FIELDSa} techfieldcost[a,'water']*(perbasea*fieldausage[a]) + sum {b in FIELDSb} techfieldcost[b,'water']*(perbaseb*fieldbusage[b]) + 402*(halfbasea+halfbaseb) + 800*(bothbases));
  
#
# constraints
#

subject to assignfield{j in FIELDS}:  #  assign crops to fields
  sum{i in CROPS} grow[i,j] = 1;
  
subject to assigntech{i in CROPS, j in FIELDS, k in TECHNOLOGIES}:  #  assign technologies to crops
  usetech[i,j,k] <= grow[i,j];
  
subject to totaltech{k in TECHNOLOGIES}: # calculate amount spent on each technology
  sum{i in CROPS, j in FIELDS} techfieldcost[j,k]*usetech[i,j,k]
    = techsum[k];
    
subject to techavail{k in TECHNOLOGIES:k <> 'water'}: techsum[k] <= availability[k];
    
subject to findcosts: techcosts = sum{k in TECHNOLOGIES:k <> 'water'} techsum[k];

subject to findrevenue{i in CROPS}:
  sum{j in FIELDS} yield[i,j] * ( grow[i,j] + sum{k in TECHNOLOGIES} improve[i,k]*usetech[i,j,k] )
    = revenue[i];
    
subject to sumrevenue: totalrevenue = sum{i in CROPS} revenue[i];

subject to respectLB{i in CROPS}: revenue[i] >= propLB[i]*totalrevenue;
subject to respectUB{i in CROPS}: revenue[i] <= propUB[i]*totalrevenue;

subject to BinarytechFieldsum{f in FIELDS, t in TECHNOLOGIES}: sum{c in CROPS} usetech[c,f,t]= techbin[t,f];

subject to TechField{f in FIELDS}: sum{t in TECHNOLOGIES} techbin[t,f] = 1; 

subject to BinarytechFieldsUB{f in FIELDS, c in CROPS, t in TECHNOLOGIES}: usetech[c,f,t] <= techbin[t,f];   

subject to TechLimitedUBa{t in TECHlimited, f in FIELDSa}: sum{c in CROPS} usetech[c,f,t] <= limited[t]; 

subject to TechLimitedUBb{t in TECHlimited, f in FIELDSb}: sum{c in CROPS} usetech[c,f,t] <= (1-limited[t]);

subject to Basealimits: halfbasea+perbasea+bothbases = 1;

subject to Baseblimits: halfbaseb+perbaseb+bothbases = 1;

subject to not2half: halfbasea + halfbaseb<= 1;

subject to notperaboth: perbasea + bothbases <= 1;

subject to notperbboth: perbaseb + bothbases <= 1;

subject to assignwaterFielda{a in FIELDSa}: fieldausage[a] = sum{c in CROPS} usetech[c,a,'water'];

subject to assignwaterFieldb{b in FIELDSb}: fieldbusage[b] = sum{c in CROPS} usetech[c,b,'water'];

subject to perabothineq: sum{a in FIELDSa} fieldausage[a] >= 10*bothbases;

subject to perbbothineq: sum{b in FIELDSb} fieldbusage[b] >= 10*bothbases;
 
