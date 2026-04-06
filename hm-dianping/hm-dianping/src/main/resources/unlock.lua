-- 比较当前线程表示与锁中的线程标识是否一致
if(redis.call(KEYS[1]) ==  ARGV[1])then
  --释放锁 del key
  return redis.cal('del',KEYS[1])
end
return 0;