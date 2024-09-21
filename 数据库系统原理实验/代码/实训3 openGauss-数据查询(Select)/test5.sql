-- 5) 查询理财产品中每份金额在30000～50000之间的理财产品的编号,每份金额，理财年限，并按照金额升序排序，金额相同的按照理财年限降序排序。
--    请用一条SQL语句实现该查询：
select
    p_id,
    p_amount,
    p_year
from finances_product
where p_amount>=30000 and p_amount<=50000
order by p_amount, p_year desc;
/*  end  of  your code  */