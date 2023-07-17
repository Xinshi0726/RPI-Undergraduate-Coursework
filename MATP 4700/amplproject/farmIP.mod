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
