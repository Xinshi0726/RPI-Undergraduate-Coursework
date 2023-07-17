method dutch(arr: array?<char>) returns (k: int)
    requires arr != null
    requires forall a:: 0<=a<arr.Length ==> arr[a] == 'b' || arr[a] == 'r'
    requires arr.Length >= 1
    ensures 0<=k<=arr.Length
    ensures forall e:: 0<=e<k ==> arr[e] == 'r'
    ensures forall f:: k<=f<arr.Length ==> arr[f] == 'b'
    modifies arr 
    {
    var i := 0;
    var j := arr.Length-1;
    while (i <= j)
    invariant forall d:: 0<=d<=arr.Length-1 ==> arr[d] == 'b' || arr[d] == 'r'
    invariant 0<=i<=arr.Length
    invariant -1<=j<=arr.Length-1
    invariant forall b:: 0<=b<i ==> arr[b] == 'r' 
    invariant forall c:: j<c<arr.Length ==> arr[c] == 'b'
    decreases j-i
    {
        if arr[i] == 'b' && arr[j] == 'r'{
            arr[i] := 'r';
            arr[j] := 'b';
            i := i+1;
            j := j-1;
        }
        else if arr[i] == 'r'&& arr[j] == 'r'{
            i := i+1;
        }
        else if arr[i] == 'b' && arr[j] == 'b'{
            j := j-1;
      }
        else if arr[i] == 'r' && arr[j] == 'b'{
            i := i+1;
            j := j-1;
        }
        else{
          i := i+1;
          j := j-1;
        }
    }
    k := j+1;
    }