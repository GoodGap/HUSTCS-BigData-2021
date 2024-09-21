  -- 20) 查询销售总额前三的理财产品
--   请用一条SQL语句实现该查询：
select
  pyear,
  rank() over(partition by pyear order by sumamount desc) as rk,
  p_id,
  sum(buyamount) as sumamount
from
(select
  to_char(pro_purchase_time, ' yyyy') as pyear,
  p_id,
  sum(pro_quantity*p_amount) as buyamount
from property, finances_product
where p_id=pro_pif_id
and pro_purchase_time like '2010%'
and pro_type=1
group by pyear, p_id

union
(select
  to_char(pro_purchase_time, ' yyyy') as pyear,
  p_id,
  sum(pro_quantity*p_amount) as buyamount
from property, finances_product
where p_id=pro_pif_id
and pro_purchase_time like '2011%'
and pro_type=1
group by pyear, p_id
))

group by pyear,p_id
order by pyear,rk, p_id
limit 6;
--/*  end  of  your code  */