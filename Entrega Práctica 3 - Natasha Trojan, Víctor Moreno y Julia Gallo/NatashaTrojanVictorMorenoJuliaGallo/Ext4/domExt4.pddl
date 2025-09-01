(define (domain planificacion-contenidos)
  (:requirements :adl :typing :existential-preconditions :fluents)

  (:types
    contenido
    dia
  )

  (:functions
    (numero_dia ?dia - dia)
    (planificado_para ?contenido - contenido)
    (minutos_consumidos ?dia - dia) ; Función para almacenar los minutos consumidos en un día
    (duracion ?contenido - contenido) ; Función para la duración de cada contenido
  )

  (:predicates
    (visto ?contenido - contenido)
    (predecesor ?contenido - contenido ?pre - contenido)
    (paralelo ?contenido - contenido ?par - contenido)
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

  (:action querer_paralelo
    :parameters (?contenido - contenido ?par - contenido)
    :precondition (and
                    (not (quiere_ver ?par))
                    (paralelo ?contenido ?par)
                    (quiere_ver ?contenido)
                  )
    :effect (and
              (quiere_ver ?par)
            )
  )

  (:action ver
    :parameters (?contenido - contenido ?dia - dia)
    :precondition (and
                    (quiere_ver ?contenido)
                    (not (visto ?contenido))
                    (<= (+ (minutos_consumidos ?dia) (duracion ?contenido)) 200) ; Verificar si no se superan los 200 minutos
                    (or
                      (not (exists (?x - contenido) (predecesor ?contenido ?x)))
                      (forall (?pre - contenido)
                        (or
                          (not (predecesor ?contenido ?pre))
                          (and
                            (predecesor ?contenido ?pre)
                            (visto ?pre)
                            (< (planificado_para ?pre) (numero_dia ?dia))
                          )
                        )
                      )
                    )
                    (forall (?par - contenido)
                      (or
                        (not (paralelo ?contenido ?par))
                        (and
                          (paralelo ?contenido ?par)
                          (visto ?par)
                          (<= (numero_dia ?dia) (planificado_para ?par))
                          (>= (numero_dia ?dia) (planificado_para ?par))
                        )
                      )
                    )
                  )
    :effect (and
              (visto ?contenido)
              (assign (planificado_para ?contenido) (numero_dia ?dia))
              (increase (minutos_consumidos ?dia) (duracion ?contenido)) ; Actualizar los minutos consumidos
            )
  )
)
