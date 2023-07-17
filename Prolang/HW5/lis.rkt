;;lis_slow
(define (nond list)
  (if (null? (cdr list)) #t
     (and (not (> (car list) (cadr list)))
           (nond (cdr list)))
      ))
        
(define (subsets L)
 (if (null? L)
 (list '())
 (extend (subsets (cdr L))
(car L))
 )
)

(define (extend L E)
 (append L (distrib L E))
)

(define (distrib L E)
 (if (null? L)
 '()
 (cons (cons E (car L))
 (distrib (cdr L) E))
 )
)

(define (gets ol lis lens)
  (if (and (equal? (length (car lis)) lens)
           (nond (car lis)))
      (car lis) (if
                 (not (null? (cdr lis))) (gets ol (cdr lis) lens)
                 (gets ol ol (- lens 1)))
      )
  )

(define (lis_slow lis)
  (let ((x (subsets lis))) (gets x x (length lis))))

;; lis_fast
(define (lismax seq val i max-seq max-v max-i end)
  (cond ((eq? i end) (list max-v max-i))
        ((and (> (+ (list-ref val i) 1) max-v) (<= (list-ref seq i) max-seq))
           (lismax seq val (+ i 1) max-seq (+ (list-ref val i) 1) i end))
        (else (lismax seq val (+ i 1) max-seq max-v max-i end))
      )
)


(define (fast_helper seq i lens i_s max-v max-i)
  (cond ((eq? i (length seq)) (list i_s max-i))
        (else (let ((ret (lismax seq lens 0 (list-ref seq i) 1 -1 i)))
                (fast_helper seq (+ i 1) (append lens (list (car ret))) (append i_s (list (cadr ret)))
                      (max (car ret) max-v) (if (> (car ret) max-v) i max-i))
   ))
  )
)



(define (findret seq ls index)
  (cond ((equal? index -1) '())
        (else (cons (list-ref seq index)
                    (findret seq ls (list-ref ls index)))
              )
  )
)

;; Contract: (list int) -> (list int)
;; Purpose: a list's non-decreasing sub list
;; Example: (lis_fast '(1 2 3 4 3 4 5)) should produce '(1 2 3 4 5)
;; Definition:
(define (lis_fast seq) '()
 (let ((ret (fast_helper seq 0 '() '() 0 -1)))
    (reverse (findret seq (car ret) (cadr ret)))
 )
)