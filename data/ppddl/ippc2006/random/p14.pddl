(define (domain prob_domain) 
 (:requirements :strips :probabilistic-effects :conditional-effects) 
 (:constants GV GF OL MP GQ SH YK CF OC QB )
 (:predicates 
	 (AT ?X ?Y ) 
	 (DS ?X ) 
	 (KP ?X ) 
	 (MF ?X ?Y ) 
	 (SD ?X ?Y ) 
	 (NY ?X ?Y ) 
	 (QW ?X ?Y ) 
	 (NJ ?X ?Y ) 
	 (GU ?X ?Y ) 
	 (PU ?X ) 
(clear)
(not-clear)
 )
(:action QPO
 :parameters (?X )
 :precondition (and 
		 (NY ?X ?X) 
		 (SD ?X ?X) 
		 (MF ?X ?X) 
  )
 :effect (probabilistic 
		 91/100 (and (not (NY ?X ?X)) )  
		 9/100 (and (GU ?X ?X) (AT ?X ?X) )  
          )
 )
(:action AHG
 :parameters (?X ?Y ?Z )
 :precondition (and 
		 (AT ?Y ?Y) 
		 (AT ?Y ?X) 
		 (PU ?Y) 
  )
 :effect (probabilistic 
		 70/100 (and (NY ?Z ?Z) (PU ?Z) )  
		 30/100 (and (SD ?Z ?Z) (NY ?Z ?X) (not (PU ?Y)) (KP ?X) (not (AT ?Y ?X)) )  
          )
 )
(:action IPU
 :parameters (?X ?Y ?Z )
 :precondition (and 
		 (DS ?Z) 
		 (GU ?Z ?Y) 
  )
 :effect (probabilistic 
		 100/100 (and (PU ?Z) (KP ?X) (NJ ?X ?Z) (AT ?X ?Y) (not (DS ?Z)) )  
          )
 )
(:action XLL
 :parameters (?X ?Y ?Z )
 :precondition (and 
		 (MF ?Z ?Z) 
  )
 :effect (probabilistic 
		 8/100 (and (NY ?Y ?Z) )  
		 92/100 (and (NY ?X ?X) (PU ?Y) (not (MF ?Z ?Z)) )  
          )
 )
(:action AYX
 :parameters (?X ?Y ?Z )
 :precondition (and 
		 (GU ?Z ?X) 
		 (GU ?X ?Z) 
		 (SD ?Z ?X) 
  )
 :effect (probabilistic 
		 91/100 (and (QW ?Z ?X) (NY ?Z ?Y) (DS ?Z) (QW ?X ?X) )  
		 4/100 (and (not (SD ?Z ?X)) (PU ?Y) )  
		 5/100 (and (PU ?Z) (PU ?X) (not (SD ?Z ?X)) )  
          )
 )
(:action IJP
 :parameters (?X )
 :precondition (and 
		 (GU ?X ?X) 
		 (KP ?X) 
  )
 :effect (probabilistic 
		 100/100 (and (NY ?X ?X) (NJ ?X ?X) (not (KP ?X)) (not (GU ?X ?X)) )  
          )
 )
(:action XAV
 :parameters (?X ?Y )
 :precondition (and 
		 (SD ?X ?X) 
  )
 :effect (probabilistic 
		 100/100 (and (QW ?Y ?Y) (NJ ?X ?Y) )  
          )
 )
(:action TUB
 :parameters (?X )
 :precondition (and 
		 (AT ?X ?X) 
		 (NY ?X ?X) 
  )
 :effect (probabilistic 
		 100/100 (and (MF ?X ?X) (KP ?X) (GU ?X ?X) (QW ?X ?X) )  
          )
 )
(:action GGW
 :parameters (?X ?Y ?Z )
 :precondition (and 
		 (NY ?X ?X) 
		 (AT ?Y ?Z) 
  )
 :effect (probabilistic 
		 100/100 (and (SD ?Z ?X) (AT ?Y ?X) )  
          )
 )
(:action VFR
 :parameters (?X ?Y )
 :precondition (and 
		 (NY ?Y ?X) 
		 (DS ?Y) 
		 (MF ?Y ?X) 
  )
 :effect (probabilistic 
		 100/100 (and (NJ ?X ?X) (SD ?X ?Y) )  
          )
 )
(:action reset1 
 :precondition (not-clear)
 :effect (and 
	     (forall (?x) (and 
      (not (AT ?x GV)) 
      (not (AT ?x GF)) 
      (not (AT ?x OL)) 
      (not (AT ?x MP)) 
      (not (AT ?x GQ)) 
      (not (AT ?x SH)) 
      (not (AT ?x YK)) 
      (not (AT ?x CF)) 
      (not (AT ?x OC)) 
      (not (AT ?x QB)) 
      (not (DS ?x)) 
      (not (KP ?x)) 
      (not (MF ?x GV)) 
      (not (MF ?x GF)) 
      (not (MF ?x OL)) 
      (not (MF ?x MP)) 
      (not (MF ?x GQ)) 
      (not (MF ?x SH)) 
      (not (MF ?x YK)) 
      (not (MF ?x CF)) 
      (not (MF ?x OC)) 
      (not (MF ?x QB)) 
      (not (SD ?x GV)) 
      (not (SD ?x GF)) 
      (not (SD ?x OL)) 
      (not (SD ?x MP)) 
      (not (SD ?x GQ)) 
      (not (SD ?x SH)) 
      (not (SD ?x YK)) 
      (not (SD ?x CF)) 
      (not (SD ?x OC)) 
      (not (SD ?x QB)) 
      (not (NY ?x GV)) 
      (not (NY ?x GF)) 
      (not (NY ?x OL)) 
      (not (NY ?x MP)) 
      (not (NY ?x GQ)) 
      (not (NY ?x SH)) 
      (not (NY ?x YK)) 
      (not (NY ?x CF)) 
      (not (NY ?x OC)) 
      (not (NY ?x QB)) 
      (not (QW ?x GV)) 
      (not (QW ?x GF)) 
      (not (QW ?x OL)) 
      (not (QW ?x MP)) 
      (not (QW ?x GQ)) 
      (not (QW ?x SH)) 
      (not (QW ?x YK)) 
      (not (QW ?x CF)) 
      (not (QW ?x OC)) 
      (not (QW ?x QB)) 
      (not (NJ ?x GV)) 
      (not (NJ ?x GF)) 
      (not (NJ ?x OL)) 
      (not (NJ ?x MP)) 
      (not (NJ ?x GQ)) 
      (not (NJ ?x SH)) 
      (not (NJ ?x YK)) 
      (not (NJ ?x CF)) 
      (not (NJ ?x OC)) 
      (not (NJ ?x QB)) 
      (not (GU ?x GV)) 
      (not (GU ?x GF)) 
      (not (GU ?x OL)) 
      (not (GU ?x MP)) 
      (not (GU ?x GQ)) 
      (not (GU ?x SH)) 
      (not (GU ?x YK)) 
      (not (GU ?x CF)) 
      (not (GU ?x OC)) 
      (not (GU ?x QB)) 
      (not (PU ?x)) 
))
(not (not-clear))
(clear)))

(:action reset2 
 :precondition (clear) 
 :effect (and (not-clear)
              (not (clear))
(NJ GQ QB) 
(NJ SH MP) 
(NJ SH GV) 
(GU OC QB) 
(PU OC) 
(SD GQ GV) 
(GU QB GV) 
(GU GQ SH) 
(AT GQ SH) 
(NJ YK OC) 
(PU GQ) 
(SD MP YK) 
(SD OC QB) 
(SD YK OC) 
(MF CF OL) 
(PU QB) 
(KP QB) 
(PU GV) 
(NY GQ OL) 
(DS OL) 
(PU CF) 
(AT GV MP) 
(NJ MP GF) 
(NY QB GF) 
(SD SH QB) 
(KP GF) 
(QW YK GV) 
(NJ OC GF) 
(QW GV OC) 
(MF OL QB) 
(NY MP QB) 
(NJ YK OL) 
(QW GQ GF) 
(DS SH) 
(NJ MP OC) 
(MF OC CF) 
(GU QB YK) 
(SD OL MP) 
(NY SH OL) 
(NY GQ CF) 
(AT SH GQ) 
(NY GQ MP) 
(DS CF) 
(PU SH) 
(GU OL OL) 
(NJ SH OL) 
(DS YK) 
(AT QB CF) 
(QW SH GF) 
(DS GV) 
(MF GV GV) 
(GU YK SH) 
(MF QB CF) 
(AT GV GQ) 
(AT MP QB) 
(GU GV GF) 
(DS OC) 
(GU QB CF) 
(PU GF) 
(GU OC YK) 
(GU GF QB) 
(GU GV SH) 
(NY MP SH) 
(SD SH SH) 
(DS GF) 
(QW QB SH) 
(GU CF GQ) 
(QW QB CF) 
(QW CF GQ) 
(SD QB MP) 
(SD GF OC) 
(PU YK) 
(MF GQ MP) 
(GU QB SH) 
(QW OL OC) 
(KP GQ) 
(MF GF MP) 
(GU QB OL) 
(NJ GQ SH) 
(GU GV GV) 
(SD MP GF) 
(PU MP) 
(MF YK QB) 
(NY SH GQ) 
(NJ SH OC) 
(QW CF YK) 
(SD OL GQ) 
(MF YK GF) 
(NY CF CF) 
(AT GQ YK) 
(NJ QB OL) 
(DS GQ) 
(GU CF MP) 
(QW OL SH) 
(QW OL MP) 
(SD YK QB) 
(GU MP YK) 
(NY MP GV) 
(MF GV GF) 
(GU GF MP) 
)))
(define (problem random-problem999) 
 (:domain prob_domain) 
 (:init 
(not-clear)
(NJ GQ QB) (NJ SH MP) (NJ SH GV) (GU OC QB) (PU OC) (SD GQ GV) (GU QB GV) (GU GQ SH) (AT GQ SH) (NJ YK OC) (PU GQ) (SD MP YK) (SD OC QB) (SD YK OC) (MF CF OL) (PU QB) (KP QB) (PU GV) (NY GQ OL) (DS OL) (PU CF) (AT GV MP) (NJ MP GF) (NY QB GF) (SD SH QB) (KP GF) (QW YK GV) (NJ OC GF) (QW GV OC) (MF OL QB) (NY MP QB) (NJ YK OL) (QW GQ GF) (DS SH) (NJ MP OC) (MF OC CF) (GU QB YK) (SD OL MP) (NY SH OL) (NY GQ CF) (AT SH GQ) (NY GQ MP) (DS CF) (PU SH) (GU OL OL) (NJ SH OL) (DS YK) (AT QB CF) (QW SH GF) (DS GV) (MF GV GV) (GU YK SH) (MF QB CF) (AT GV GQ) (AT MP QB) (GU GV GF) (DS OC) (GU QB CF) (PU GF) (GU OC YK) (GU GF QB) (GU GV SH) (NY MP SH) (SD SH SH) (DS GF) (QW QB SH) (GU CF GQ) (QW QB CF) (QW CF GQ) (SD QB MP) (SD GF OC) (PU YK) (MF GQ MP) (GU QB SH) (QW OL OC) (KP GQ) (MF GF MP) (GU QB OL) (NJ GQ SH) (GU GV GV) (SD MP GF) (PU MP) (MF YK QB) (NY SH GQ) (NJ SH OC) (QW CF YK) (SD OL GQ) (MF YK GF) (NY CF CF) (AT GQ YK) (NJ QB OL) (DS GQ) (GU CF MP) (QW OL SH) (QW OL MP) (SD YK QB) (GU MP YK) (NY MP GV) (MF GV GF) (GU GF MP)  
)
 (:goal (and 
(NY MP  MP ) 
(QW GV  GV ) 
(NJ MP  CF ) 
(PU OL ) 
(NJ OL  OL ) 
(NJ YK  GF ) 
(NJ OC  GV ) 
(NJ GQ  GQ ) 
(QW GF  GF ) 
(NJ SH  GF ) 
(NJ MP  YK ) 
(NJ CF  OC ) 
(NY CF  SH ) 
(QW CF  CF ) 
(NJ CF  CF ) 
(QW OL  OL ) 
(NJ CF  OL ) 
(QW SH  SH ) 
(NJ MP  SH ) 
(NY OL  OL ) 
(NY OL  MP ) 
)))