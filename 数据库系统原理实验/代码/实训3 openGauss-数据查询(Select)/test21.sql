   -- 21) 投资积极且偏好理财类产品的客户
--   请用一条SQL语句实现该查询：
select a.pro_c_id
from
(
select count(distinct pro_pif_id) as fi_num, pro_c_id
from property, finances_product
where pro_pif_id=p_id and pro_type=1
group by pro_c_id
having fi_num>3
)a,
(
select count(distinct pro_pif_id) as f_num, pro_c_id
from property, fund
where pro_pif_id=f_id and pro_type=3
group by pro_c_id
)b
where a.pro_c_id=b.pro_c_id and fi_num>f_num
order by pro_c_id desc;
--/*  end  of  your code  */