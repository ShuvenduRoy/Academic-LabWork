parent(shumailia,mubashir).
parent(sumiya,ali).

parent(afaque,ali).
parent(shoaib,mubashir).

female(shumailia).
female(sumiya).

male(afaque).
male(shoaib).

mother(X,Y):-
	parent(X,Y),female(X).

father(X,Y):-
	parent(X,Y),male(X).
