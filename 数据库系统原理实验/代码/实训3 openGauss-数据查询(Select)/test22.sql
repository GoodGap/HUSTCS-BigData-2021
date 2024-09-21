    -- 22) 查询购买了所有畅销理财产品的客户
--   请用一条SQL语句实现该查询：
select
    distinct pro_c_id
from property a
where not exists
(
    select *
    from property b
    where b.pro_pif_id in
    (
        select
            pro_pif_id
        from property
        where pro_type=1
        group by pro_pif_id
        having count(*)>2
    )
    and not exists
    (
        select *
        from property c
        where c.pro_c_id=a.pro_c_id
        and c.pro_pif_id=b.pro_pif_id
        and c.pro_type=1
    )
)
order by pro_c_id;
--/*  end  of  your code  */