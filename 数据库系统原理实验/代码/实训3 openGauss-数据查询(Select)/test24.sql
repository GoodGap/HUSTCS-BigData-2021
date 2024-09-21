      -- 24) 查询任意两个客户的相同理财产品数
--   请用一条SQL语句实现该查询：
select
      a.pro_c_id as pro_c_id,
      b.pro_c_id as pro_c_id,
      count(*) as total_count
from property a, property b
where a.pro_c_id <> b.pro_c_id
and a.pro_pif_id=b.pro_pif_id
and a.pro_type=1
and b.pro_type=1
group by a.pro_c_id, b.pro_c_id
having total_count>=2
order by a.pro_c_id;
--/*  end  of  your code  */