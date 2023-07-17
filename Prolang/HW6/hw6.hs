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
---- Terms ----
{-
x = Var "x"
y = Var "y"
z = Var "z"
xx = Lambda "x" (App (Var "x") (Var "x"))
i = Lambda "x" (Var "x")
s = Lambda "x" (Lambda "y" (Lambda "z" (App (App x z) (App y z))))
s_x = Lambda "y" (Lambda "z" (App (App x z) (App y z)))
-}
---- Functions ----
freshVars::Expr -> [Name]
---- YOUR CODE HERE
freshVars e = (map (\x->(show x)++"_") [1..]) \\ freeVars(e)  
subst::(Name,Expr) -> Expr -> Expr
---- YOUR CODE HERE
subst (v,e) (Var n) = if v==n then e else (Var n)
subst (v,e) (App e1 e2) = (App (subst (v,e) e1) (subst (v,e) e2))
subst (v,e) (Lambda n e1) =
  if v==n then
    Lambda n e1
  else
    let z = head ((map (\x->(show x)++"_") [1..]) \\ (nub (freeVars(e)++[v]++freeVars(e1))))
    in
      Lambda z (subst (v,e) (subst (n,(Var z)) e1))
normNF_OneStep::Expr -> Maybe Expr
---- YOUR CODE HERE
normNF_OneStep (Var n) = Nothing
normNF_OneStep (Lambda n e) =
  case (normNF_OneStep e) of
    Nothing -> Nothing
    Just e1 -> Just (Lambda n e1)
normNF_OneStep (App e1 e2) =
  case e1 of
    Lambda n e3 -> Just (subst (n,e2) e3)
    _ -> case (normNF_OneStep e1) of
           Nothing -> case (normNF_OneStep e2) of
                        Nothing -> Nothing
                        Just e2prime -> Just (App e1 e2prime)
           Just e1prime -> Just (App e1prime e2)
normNF_n::Int -> Expr -> Expr
---- YOUR CODE HERE
normNF_n n e =
  if n==0 then
    e
  else
    case (normNF_OneStep e) of
      Nothing -> normNF_n (n-1) e
      Just e1 -> normNF_n (n-1) e1 
--- HELPERS ----
freeVars::Expr -> [Name]
freeVars (Var n) = [n]
freeVars (App e1 e2) = nub ((freeVars e1)++(freeVars e2))
freeVars (Lambda n e) = (freeVars e) \\ [n]