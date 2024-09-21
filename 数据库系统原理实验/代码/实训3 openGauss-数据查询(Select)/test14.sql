-- 14) 查询每份保险金额第4高保险产品的编号和保险金额。
--     在数字序列8000,8000,7000,7000,6000中，
--     两个8000均为第1高，两个7000均为第2高,6000为第3高。
-- 请用一条SQL语句实现该查询：
select
    i_id,
    i_amount
from insurance
where i_amount = (
    select
        distinct i_amount
    from insurance
    order by i_amount desc
    limit 3, 1
);
/*  end  of  your code  */