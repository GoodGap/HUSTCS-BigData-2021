-- 19) 以日历表格式列出2022年2月每周每日基金购买总金额，输出格式如下：
-- week_of_trading Monday Tuesday Wednesday Thursday Friday
--               1
--               2    
--               3
--               4
--   请用一条SQL语句实现该查询：
select
    wk as week_of_trading,
    sum(case when daypos = 1 then amount else null end) as monday,
    sum(case when daypos = 2 then amount else null end) as tuesday,
    sum(case when daypos = 3 then amount else null end) as wendnesday,
    sum(case when daypos = 4 then amount else null end) as thursday,
    sum(case when daypos = 5 then amount else null end) as friday
from (
    select
        date_part('week',pro_purchase_time) -5 as wk,
        extract(DOW FROM cast(pro_purchase_time as TIMESTAMP)) as daypos,
        sum(pro_quantity * f_amount) as amount
    from
        property
        join fund
        on pro_pif_id = f_id
    where
        pro_purchase_time like '2022-02-%'
        and pro_type = 3
    group by pro_purchase_time
)
group by wk
order by wk;
/*  end  of  your code  */