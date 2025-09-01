(define (domain planificacion-contenidos)
  (:requirements :adl :typing :existential-preconditions :disjunctive-preconditions :fluents)

  (:types
    contenido
    dia
  )

  (:functions
    (numero_dia ?dia - dia)
    (planificado_para ?contenido - contenido)
  )

  (:predicates
    (visto ?contenido - contenido)
    (predecesor ?contenido - contenido ?pre - contenido)
    (quiere_ver ?contenido - contenido)
  )

  (:action querer_predecesor
    :parameters (?contenido - contenido ?pre - contenido)
    :precondition (and
                    (not (quiere_ver ?pre))
                    (predecesor ?contenido ?pre)
                    (quiere_ver ?contenido)
                  )
    :effect (and
              (quiere_ver ?pre)
            )
  )

  (:action ver_sin_predecesor
    :parameters (?contenido - contenido ?dia - dia)
    :precondition (and
                    (quiere_ver ?contenido)
                    (not (visto ?contenido))
                    (not (exists (?x - contenido) (predecesor ?contenido ?x)))
                  )
    :effect (and
              (visto ?contenido)
              (assign (planificado_para ?contenido) (numero_dia ?dia))
            )
  )

  (:action ver_con_predecesor
    :parameters (?contenido - contenido ?pre - contenido ?dia - dia)
    :precondition (and
                    (quiere_ver ?contenido)
                    (not (visto ?contenido))
                    (and
                      (predecesor ?contenido ?pre) (visto ?pre)
                      (< (planificado_para ?pre) (numero_dia ?dia))
                    )
                  )
    :effect (and
              (visto ?contenido)
              (assign (planificado_para ?contenido) (numero_dia ?dia))
            )
  )
)
