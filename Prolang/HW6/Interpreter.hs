-- Problem 1:
--    interpret(x) = x
--    interpret(\x.E1) = let f = interpret(E1)
--                        -> \x.f
--    interpret(E1 E2) = let f = interpret(E1)
--                        in case f of
--                            \x.E3 -> interpret(E3[E2/x])
--                            _ -> f E2

module Interpreter where
import Control.Exception
import Debug.Trace
import Data.List

---- Data types ----

type Name = String

data Expr = 
  Var Name
  | Lambda Name Expr
  | App Expr Expr
  deriving 
    (Eq,Show)


---- Functions ----
-- Purpose: Find all the free variables given a lambda calculus expression
-- Example: freeVars (App (Var "x") (Var "x")) returns ["x"]
-- Definition: freeVars :: Expr -> [Name] (list of strings)
freeVars :: Expr -> [Name]
---- YOUR CODE HERE 
freeVars s = case s of
             Var s -> [s]
             App expr1 expr2 -> union (freeVars expr1) (freeVars expr2)
             Lambda s expr -> (filter (/= s) (freeVars expr)) 

-- Purpose: This function substitues expr1 by x in expr2 and replace the bound variable by a fresh variable during the process
-- Example: subst ("z", Var "x")(App (App (Lambda "f" (Lambda "x" (Var "f")))  (Var "f")) (Var "x")) will return
-- App (App (Lambda "1_" (Lambda "2_" (Var "1_"))) (Var "f")) (Var "x")
-- Definition: subst::(Name,Expr) -> Expr -> Expr
subst::(Name,Expr) -> Expr -> Expr
---- YOUR CODE HERE
subst (x,expr1) expr2 = case expr2 of
                         Var y -> if x == y 
                          then expr1
                          else expr2
                         App expr3 expr4 -> App (subst (x,expr1) expr3) (subst (x,expr1) expr4)
                         Lambda y expr5 -> if x == y
                                          then Lambda y expr5
                                          else 
                                                let freshVar = head (map(\x->(show x) ++ "_")[1..] \\ (union (union (freeVars expr5) (freeVars expr1)) (freeVars (Var x))))
                                                in Lambda freshVar (subst (x,expr1) (subst (y,Var freshVar) expr5))

-- Purpose: This function implements the modified intepreter from lecture 20 page 4. 
-- This function produce a onestep normal order reduction if a redex exists. Otherwise it will return Nothing
-- Example: normNF_OneStep (App (App (Lambda "f" (Lambda "x" (Var "f")))  (Var "f")) (Var "x")) will return
-- Just (App (Lambda "1_" (Var "f")) (Var "x"))
-- Definition: normNF_OneStep::Expr -> Maybe Expr
normNF_OneStep::Expr -> Maybe Expr
-- ---- YOUR CODE HERE
normNF_OneStep exp =
  case exp of
    Var _ -> Nothing
    Lambda n e ->
      let e1 = normNF_OneStep e
       in case e1 of
            Just en -> Just (Lambda n en)
            _ -> Nothing
    App e1 e2 ->
      case e1 of 
        Lambda n en -> Just (subst (n,e2) en)
        _ ->  let f = normNF_OneStep e1
              in case f of
                Just en2 -> Just (App en2 e2)
                _ -> let a = normNF_OneStep e2 in case a of
                          Just en1 -> Just (App e1 en1)
                          _ -> Nothing
                

-- Purpose: Do i times normal order single step reduction on expr and return the result
-- Example: normNF_n 10 (App (App (Lambda "f" (Lambda "x" (Var "f")))  (Var "f")) (Var "x")) will return
-- Var "f"
-- Definition: normNF_n::Int -> Expr -> Expr
normNF_n::Int -> Expr -> Expr
---- YOUR CODE HERE
normNF_n i expr = if i == 0 then
  expr else
    let en = normNF_OneStep expr
     in case en of
          Just x -> normNF_n (i - 1) x
          _ -> normNF_n (i - 1) expr
