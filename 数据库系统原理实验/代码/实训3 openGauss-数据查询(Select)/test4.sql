-- 4) 查询办理了储蓄卡的客户名称、手机号、银行卡号。 查询结果结果依客户编号排序。
--    请用一条SQL语句实现该查询：
select
    c_name,
    c_phone,
    b_number
from client, bank_card
where c_id=b_c_id
and b_type='储蓄卡'
order by c_id;
/*  end  of  your code  */