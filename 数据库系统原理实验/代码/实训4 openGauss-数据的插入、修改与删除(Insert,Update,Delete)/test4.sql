-- 请用一条SQL语句删除client表中没有银行卡的客户信息：
delete
from client
where c_id not in
(
select
    c_id
from client, bank_card
where c_id=b_c_id
);
/* the end of your code */