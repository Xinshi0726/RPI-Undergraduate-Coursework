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

#
# parameters
#
var grow{CROPS,FIELDS} >= 0, <= 1;
# proportion of field j taken by crop i.
var usetech{CROPS,FIELDS,TECHNOLOGIES} >= 0, <=1;
# proportion of field j in crop i with technology k

param yield{CROPS,FIELDS} >= 0;  # expected revenue from filling the field with the crop

param availability{TECHNOLOGIES} >= 0;  # maximum availability of each technology

param techfieldcost{FIELDS,TECHNOLOGIES} >= 0;  # cost to cover a field using a technology

var spend_on_tech{i in CROPS} =  sum{j in TECHNOLOGIES,z in FIELDS} (usetech[i,z,j]*techfieldcost[z,j]);
param improve{CROPS,TECHNOLOGIES} >= 0;   # proportional boost in expected revenue
                                          # from using the technology
                                          # (assume benefits from different
                                          # technologies are additive)
param propUB{CROPS} >= 0 default 0.5;  # maximum proportion of expected yield allocated to CROP
param propLB{CROPS} >= 0 default 0.1;  # minimum proportion of expected yield allocated to CROP

var ExpectedRevenue = sum {f in FIELDS} sum {c in CROPS} (yield[c,f] * grow[c,f] + (sum {t in TECHNOLOGIES} yield[c,f] * usetech[c,f,t] * improve[c,t]));
var ExpectedCost = sum {c in CROPS, f in FIELDS, t in TECHNOLOGIES} usetech[c,f,t] * techfieldcost[f,t];
maximize ExpectedProfit: ExpectedRevenue - ExpectedCost;
subject to A {t in TECHNOLOGIES}: sum {c in CROPS, f in FIELDS} techfieldcost[f,t] * usetech[c,f,t] <= availability[t]; # TECH contraint
subject to B {t in TECHNOLOGIES, c in CROPS, f in FIELDS}: usetech[c,f,t] <= grow[c,f]; # contraint 4.2
subject to C {f in FIELDS}: sum {c in CROPS} grow[c,f] <= 1; # sum of proportion cannot exceed 1

subject to D1 {c in CROPS}: (sum {f in FIELDS} (yield[c,f] * grow[c,f] + (sum {t in TECHNOLOGIES} yield[c,f] * usetech[c,f,t] * improve[c,t])))<= propUB[c]*ExpectedRevenue;
subject to D2 {c in CROPS}: (sum {f in FIELDS} (yield[c,f] * grow[c,f] + (sum {t in TECHNOLOGIES} yield[c,f] * usetech[c,f,t] * improve[c,t])))>= propLB[c]*ExpectedRevenue;


var TechImpForEachCrop {c in CROPS} = sum{f in FIELDS, t in TECHNOLOGIES} yield[c,f] * usetech[c,f,t] * improve[c,t];

var yield_on_each {c in CROPS} = sum{j in FIELDS} yield[c,j];

var tech_spend_on_crop{i in CROPS} =  sum{j in TECHNOLOGIES,z in FIELDS} (usetech[i,z,j]*techfieldcost[z,j]);

var TechCost {t in TECHNOLOGIES} = sum {c in CROPS, f in FIELDS} techfieldcost[f,t] * usetech[c,f,t];   # Spending on each tech