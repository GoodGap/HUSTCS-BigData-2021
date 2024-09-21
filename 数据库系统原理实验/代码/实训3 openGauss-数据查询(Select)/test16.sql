-- 16) 查询持有相同基金组合的客户对，如编号为A的客户持有的基金，编号为B的客户也持有，反过来，编号为B的客户持有的基金，编号为A的客户也持有，则(A,B)即为持有相同基金组合的二元组，请列出这样的客户对。为避免过多的重复，如果(1,2)为满足条件的元组，则不必显示(2,1)，即只显示编号小者在前的那一对，这一组客户编号分别命名为c_id1,c_id2。

-- 请用一条SQL语句实现该查询：
select distinct c_id1, c_id2
from
(select a.pro_c_id as c_id1, b.pro_c_id as c_id2
from property a, property b, fund
where c_id1<c_id2
and not exists
(select pro_pif_id from property m
where pro_c_id=a.pro_c_id and pro_type=3
and not exists
(select * from property
where pro_c_id=b.pro_c_id and pro_pif_id=m.pro_pif_id))
and not exists
(select pro_pif_id from property n
where pro_c_id=b.pro_c_id and pro_type=3
and not exists
(select * from property
where pro_c_id=a.pro_c_id and pro_pif_id=n.pro_pif_id))
and exists
(select * from property
where pro_c_id=a.pro_c_id and pro_type=3));

/*  end  of  your code  */