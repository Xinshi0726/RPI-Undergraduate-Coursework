quantityConstraints(F, H) :- F >= 0, H >= 0, F =< 3, H =< 3.

move([LF,LH,LB]):-LB=0,LH is LH+1,LB = 1,quantityConstraints(LF,LH),P = [LF,LH,LB];
move([LF,LH,LB]):-LB=0,LF is LF+1,LB = 1,quantityConstraints(LF,LH),P = [LF,LH,LB];
move([LF,LH,LB]):-LB=1,LH is LH-2,LB = 0,quantityConstraints(LF,LH),P = [LF,LH,LB];
move([LF,LH,LB]):-LB=1,LF is LF-2,LB = 0,quantityConstraints(LF,LH),P = [LF,LH,LB];
move([LF,LH,LB]):-LB=1,LH is LH-1, LF = LF-1,LB = 0,quantityConstraints(LF,LH),P = [LF,LH,LB];
move([LF,LH,LB]):-LB=1,LH is LH-1, LF = LF-1,LB = 0,quantityConstraints(LF,LH),P = [LF,LH,LB];

goal([0,0,0])
solve(P):- move([3,3,1]),\+goal(P),solve(P).