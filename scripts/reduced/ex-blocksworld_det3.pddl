(  
  (define 
    (domain exploding-blocksworld)  
    (:requirements 
     :typing 
     :conditional-effects 
     :probabilistic-effects 
     :equality 
     :rewards)  
    (:types block)  
    (:predicates 
      (on ?b1 ?b2 - block) 
      (on-table ?b - block) 
      (clear ?b - block) 
      (holding ?b - block) 
      (emptyhand) 
      (no-detonated ?b - block) 
      (no-destroyed ?b - block) 
      (no-destroyed-table) )   
    (:action pick-up 
     :parameters 
      (?b1 ?b2 - block)  
     :precondition 
      (and 
        (emptyhand) 
        (clear ?b1) 
        (on ?b1 ?b2) 
        (no-destroyed ?b1) )  
     :effect 
      (and 
        (holding ?b1) 
        (clear ?b2) 
        (not 
          (emptyhand) ) 
        (not 
          (on ?b1 ?b2) ) ) )  
    (:action pick-up-from-table 
     :parameters 
      (?b - block)  
     :precondition 
      (and 
        (emptyhand) 
        (clear ?b) 
        (on-table ?b) 
        (no-destroyed ?b) )  
     :effect 
      (and 
        (holding ?b) 
        (not 
          (emptyhand) ) 
        (not 
          (on-table ?b) ) ) )  
    (:action put-down 
     :parameters 
      (?b - block)  
     :precondition 
      (and 
        (holding ?b) 
        (no-destroyed-table) )  
     :effect 
      (and 
        (emptyhand) 
        (on-table ?b) 
        (not 
          (holding ?b) )  
        () ) )  
    (:action put-on-block 
     :parameters 
      (?b1 ?b2 - block)  
     :precondition 
      (and 
        (holding ?b1) 
        (clear ?b2) 
        (no-destroyed ?b2) )  
     :effect 
      (and 
        (emptyhand) 
        (on ?b1 ?b2) 
        (not 
          (holding ?b1) ) 
        (not 
          (clear ?b2) )  
        () ) ) )  ) 