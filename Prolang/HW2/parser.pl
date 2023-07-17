% Grammar 
% =======
% Nonterminals expr, term, term_tail and factor_tail are encoded as
% non(e,_), non(t,_), non(tt,_) and non(ft,_), respectively. 
% Special nonterminal start is encoded as non(s,_).
% Terminals num, -, and * are encoded as 
% term(num,_), term(minus,_) and term(times,_). 
% Special terminal term(eps,_) denotes the epsilon symbol.
% 
% Productions are represented as prod(N,[H|T]) where N is the unique
% index of the production, H is the left-hand-side, and T is the 
% right-hand-side. 

prod(0,[non(s,_),non(e,_)]).
prod(1,[non(e,_),non(t,_),non(tt,_)]). 
prod(2,[non(tt,_),term(minus,_),non(t,_),non(tt,_)]).
prod(3,[non(tt,_),term(eps,_)]).
prod(4,[non(t,_),term(num,_),non(ft,_)]).
prod(5,[non(ft,_),term(times,_),term(num,_),non(ft,_)]).
prod(6,[non(ft,_),term(eps,_)]).


% LL(1) parsing table
% ===================
% E.g., predict(non(s,_),term(num,_),0) stands for "on start and num, 
% predict production 0. start -> expr".


% YOUR CODE HERE. 
% Complete the LL(1) parsing table for the above grammar.
predict(non(s,_),term(num,_),0).
predict(non(e,_),term(num,_),1).
predict(non(tt,_),term(minus,_),2).
predict(non(tt,_),term(end,_),3).
predict(non(t,_),term(num,_),4).
predict(non(ft,_),term(times,_),5).
predict(non(ft,_),term(minus,_),6).
predict(non(ft,_),term(end,_),6).


% Sample inputs
% =============
input0([3,-,5]).
input1([3,-,5,*,7,-,18]).


% Transform
% ========
% Transform translates a token stream into the generic representation, 
% including the special end-of-input-marker. E.g., [3,-,5] translates 
% into [term(num,3),term(minus,_),term(num,5),term(end,_)].

% YOUR CODE HERE.
% Write transform(L,R): it takes input list L and transforms it into a
% list where terminals are represented with term(...). The transformed 
% list will be computed in unbound variable R.
% E.g., transform([3,-,5],R).
% R = [term(num,3),term(minus,_),term(num,5),term(end,_)]

% define the cases
termCheck(-, Z) :- Z = term(minus, _).
termCheck(*, Z) :- Z = term(times, _).
termCheck(X, Z) :- Z = term(num, X).
transform([], Z) :- Z = [term(end, _)].
% go through the elements in the list one by one and do classification
transform(L, P) :- [X|Y] = L, [C|R] = P, termCheck(X, C), transform(Y, R).

% parseLL
% =======
% YOUR CODE HERE.
% Write parseLL(R,ProdSeq): it takes a transformed list R and produces 
% the production sequence the predictive parser applies.
% E.g., transform([3,-,5],R),parseLL(R,ProdSeq).
% ProdSeq = [0, 1, 4, 6, 2, 4, 6, 3].

% base cases
parse(C,D,T) :- [term(A, _)|X]=C, [term(A, _)|Y]=D,parse(X, Y, T).
parse(C, Y, T) :- [term(eps,_)|X] = C, parse(X, Y, T). 
parse([], [term(end,_)], []).
% go through every element in R, predict what is next, then combine the results with previous terms as the next input
parse(X, T, Y) :- [Z|_] = T, [C|D] = X, [FT|Seq] = Y ,predict(C, Z, FT), prod(FT, [_|R]), append(R, D, NS), parse(NS, T, Seq).

parseLL(R, ProdSeq) :- parse([non(s,_)], R, ProdSeq).

% parseAndSolve
% =============
% YOUR CODE HERE.
% Write parseAndSolve, which augments parseLL with computation. 
% E.g., transform([3,-,5],R),parseAndSolve(R,ProdSeq,V).
% ProdSeq = [0, 1, 4, 6, 2, 4, 6, 3],
% V = -2.

% Attributes
attribute(0,[non(s,Vs),non(e,Ve)]) :- Vs = Ve.
attribute(1,[non(e,Ve),non(t,Vt),non(tt,Vtt)]) :- Ve is Vt - Vtt. 
attribute(2,[non(tt,Vttn),term(minus,_),non(t,Vt),non(tt,Vtt)]) :- Vttn is Vt + Vtt.
attribute(3,[non(tt,Vtt),term(eps,_)]) :- Vtt is 0.
attribute(4,[non(t,Vt),term(num,Num),non(ft,Ft)]) :- Vt is Num * Ft.
attribute(5,[non(ft,Vftn),term(times,_),term(num,Num),non(ft,Vft)]) :- Vftn is Num * Vft.
attribute(6,[non(ft,Vft),term(eps,_)]) :- Vft is 1.

% keep only the non terminals
removeTerm(C, X) :- [term(_, _)|N] = C,removeTerm(N, X).
removeTerm(C, D) :- [non(X, Y)|N]=C,[non(X, Y)|R]=D,removeTerm(N, R).
removeTerm([], _).

% Concatenate the new nonterm obtained after binding with the unconsumed nonterm to form the next non term
getNextNonTerm( [], X, X).                                   
getNextNonTerm( [X | Y], Z, [X | W]) :- getNextNonTerm( Y, Z, W).

postProcessing(V,TS, LNT,NNT):-removeTerm(TS, ALL),[X|LNT] = V, [X|_] = TS, [_|NNT] = ALL.

%define cases encountered
match(C, [], Q) :- Q = C.
match(C,D,Q) :- [term(X,Y)|P]=C, [term(X,Y)|S]=D, match(P, S, Q).
match(C,D,Q) :- [term(X,Z)|P]=C, [term(_,Y)|S]=D, not(X = Y), match([term(X,Z)|P], S, Q).
match(C,D,Q) :- [term(X,Z)|P]=C, [term(T,Y)|S]=D, not(term(X,Z) = term(T,Y)), match([term(X,Z)|P], S, Q).
match(C,D,Q) :- [non(X,Z)|P]=C, [term(_,_)|S]=D, match([non(X,Z)|P], S, Q).
match(C,D,Q) :- [non(X,Z)|P]=C, [non(T,Y)|S]=D, not(non(X,Z) = non(T,Y)), match([non(X,Z)|P], S, Q).
match(C,D,Q) :- [term(X,Z)|P]=C, [non(_,_)|S]=D, match([term(X,Z)|P], S, Q).

solveLL(_, [], []).
solveLL(R, [N|ProdSeq], V) :- prod(N, TS),match(R, TS, UR),postProcessing(V,TS,LNT,NNT),
                              getNextNonTerm(NNT, LNT, NINPUT),solveLL(UR, ProdSeq, NINPUT), attribute(N, TS), !.


parseAndSolve(R, ProdSeq, V) :- parseLL(R, ProdSeq), solveLL(R, ProdSeq, X), [Z|_] = X, non(_,V) = Z.
