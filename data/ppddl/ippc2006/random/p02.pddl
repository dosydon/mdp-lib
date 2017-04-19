(define (domain prob_domain) 
 (:requirements :strips :probabilistic-effects :conditional-effects) 
 (:constants FN CL BZ GL YV XR YY GC DC JK )
 (:predicates 
	 (AL ?X ?Y ) 
	 (HR ?X ) 
	 (WQ ?X ) 
	 (DR ?X ) 
	 (LS ?X ?Y ) 
(clear)
(not-clear)
 )
(:action SOG
 :parameters (?X ?Y )
 :precondition (and 
		 (HR ?X) 
  )
 :effect (probabilistic 
		 100/100 (and (LS ?Y ?Y) (not (HR ?X)) )  
          )
 )
(:action HGC
 :parameters (?X ?Y ?Z )
 :precondition (and 
		 (LS ?X ?Z) 
  )
 :effect (probabilistic 
		 66/100 (and (not (LS ?X ?Z)) (LS ?Y ?X) (HR ?X) )  
		 34/100 (and (not (LS ?X ?Z)) (DR ?X) )  
          )
 )
(:action AQB
 :parameters (?X )
 :precondition (and 
		 (HR ?X) 
		 (DR ?X) 
  )
 :effect (probabilistic 
		 100/100 (and (not (HR ?X)) )  
          )
 )
(:action OZH
 :parameters (?X ?Y ?Z )
 :precondition (and 
		 (HR ?X) 
		 (AL ?Y ?Z) 
  )
 :effect (probabilistic 
		 38/100 (and (LS ?Y ?X) (not (AL ?Y ?Z)) (HR ?Z) )  
		 41/100 (and (LS ?Y ?X) (DR ?X) )  
		 21/100 (and (not (HR ?X)) (AL ?Z ?Y) )  
          )
 )
(:action HME
 :parameters (?X )
 :precondition (and 
		 (LS ?X ?X) 
		 (AL ?X ?X) 
  )
 :effect (probabilistic 
		 100/100 (and (not (LS ?X ?X)) )  
          )
 )
(:action reset1 
 :precondition (not-clear)
 :effect (and 
	     (forall (?x) (and 
      (not (AL ?x FN)) 
      (not (AL ?x CL)) 
      (not (AL ?x BZ)) 
      (not (AL ?x GL)) 
      (not (AL ?x YV)) 
      (not (AL ?x XR)) 
      (not (AL ?x YY)) 
      (not (AL ?x GC)) 
      (not (AL ?x DC)) 
      (not (AL ?x JK)) 
      (not (HR ?x)) 
      (not (WQ ?x)) 
      (not (DR ?x)) 
      (not (LS ?x FN)) 
      (not (LS ?x CL)) 
      (not (LS ?x BZ)) 
      (not (LS ?x GL)) 
      (not (LS ?x YV)) 
      (not (LS ?x XR)) 
      (not (LS ?x YY)) 
      (not (LS ?x GC)) 
      (not (LS ?x DC)) 
      (not (LS ?x JK)) 
))
(not (not-clear))
(clear)))

(:action reset2 
 :precondition (clear) 
 :effect (and (not-clear)
              (not (clear))
(WQ JK) 
(LS DC BZ) 
(AL GL GL) 
(LS CL YY) 
(HR FN) 
(WQ GC) 
(LS YV GC) 
(AL FN YY) 
(DR CL) 
(WQ BZ) 
(AL YY JK) 
(LS DC DC) 
(DR JK) 
(WQ GL) 
(WQ FN) 
(LS FN YV) 
(HR GC) 
(WQ XR) 
(AL XR YV) 
(AL GL YY) 
)))
(define (problem random-problem51) 
 (:domain prob_domain) 
 (:init 
(not-clear)
(WQ JK) (LS DC BZ) (AL GL GL) (LS CL YY) (HR FN) (WQ GC) (LS YV GC) (AL FN YY) (DR CL) (WQ BZ) (AL YY JK) (LS DC DC) (DR JK) (WQ GL) (WQ FN) (LS FN YV) (HR GC) (WQ XR) (AL XR YV) (AL GL YY)  
)
 (:goal (and 
(LS YY  GC ) 
(DR GC ) 
)))
