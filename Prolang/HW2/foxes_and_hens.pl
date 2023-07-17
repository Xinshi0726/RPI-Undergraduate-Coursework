% quantity constraints
quantityConstraints(F, H) :- F >= 0, H >= 0, F =< 3, H =< 3.
% make sure there are more hens than foxes on both shores
safeLeft(F,H):-H>=F;H is 0.
safeRight(F,H):-A is 3-H,B is 3-F,A>=B.
safeRight(_,H):-A is 3-H,A is 0.

% use ! to avoid repeated results (otherwise there will be 8)
constraint(UF,UH):-quantityConstraints(UF,UH),safeLeft(UF,UH),safeRight(UF,UH),!.

% movement function.
move([LF,LH,0],[UF,UH,UB]):-UH is LH+1,UF is LF, UB is 1,constraint(UF,UH).
move([LF,LH,0],[UF,UH,UB]):-UF is LF+1,UH is LH,UB is 1,constraint(UF,UH).
move([LF,LH,0],[UF,UH,UB]):-UH is LH+1,UF is LF+1, UB is 1,constraint(UF,UH).
move([LF,LH,0],[UF,UH,UB]):-UH is LH+2,UF is LF, UB is 1,constraint(UF,UH).
move([LF,LH,1],[UF,UH,UB]):-UH is LH-2,UF is LF,UB is 0,constraint(UF,UH).
move([LF,LH,1],[UF,UH,UB]):-UF is LF-2,UH is LH,UB is 0,constraint(UF,UH).
move([LF,LH,1],[UF,UH,UB]):-UH is LH-1, UF is LF-1,UB is 0,constraint(UF,UH).
move([LF,LH,1],[UF,UH,UB]):-UH is LH-1,UF is LF,UB is 0,constraint(UF,UH).
move([LF,LH,1],[UF,UH,UB]):-UF is LF-1,UH is LH,UB is 0,constraint(UF,UH).

% reverse the result
reverse([], Y, R) :-
    R = Y.
reverse([H|T] , Y, R) :-
    reverse(T, [H|Y], R).

% goal
terminate([0,0,0]).
% solve for the problem, reverse the result since it goes from the end state to the start state.
try([TOP|REST],P):- not(terminate(TOP)),move(TOP,TEMP),not(member(TEMP, [TOP|REST])), try([TEMP|[TOP|REST]], P).
try([TOP|REST],P):- terminate(TOP),P = [TOP|REST].
solve(P):-try([[3,3,1]],R), reverse(R,[],P).