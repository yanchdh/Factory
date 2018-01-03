#ifndef LRU_CACHE_H_
#define LRU_CACHE_H_

#include <unordered_map>
#include <memory>
#include <cassert>
#include <thread>
#include <mutex>

class Lock;

template<typename KeyType, typename ValueType>
class LRUCache final
{
public:
	static const size_t kMinCacheCount_ = 1024;
	static const size_t kMaxCacheCount_ = 102400;
	
	struct DLinkedNode
	{
		DLinkedNode*				pre_;
		DLinkedNode*				nxt_;
		KeyType						key_;
		std::shared_ptr<ValueType>	val_;

		DLinkedNode() 
			: pre_(NULL)
			, nxt_(NULL)
		{
		}
		DLinkedNode(KeyType key, const std::shared_ptr<ValueType>& val) 
			: pre_(NULL)
			, nxt_(NULL)
			, key_(key)
			, val_(val)
		{
		}
		~DLinkedNode()
		{
			pre_ = nxt_ = NULL;
		}
	};
private:
	size_t		cacheBytes_;
	size_t		perNodeBytes_;
	size_t		maxCacheCount_;

	size_t		cacheGetCount_;
	size_t		cacheHitCount_;

	size_t		cacheCount_;
	DLinkedNode head_;
	DLinkedNode tail_;
	std::unordered_map<KeyType, DLinkedNode*> kv_;

	bool		islock_;
	std::mutex	mutex_;

	inline void dlinkednode_push_back(DLinkedNode* node)
	{
		node->pre_ = tail_.pre_;
		node->nxt_ = &tail_;
		tail_.pre_->nxt_ = node;
		tail_.pre_ = node;
	}

	inline void dlinkednode_remove(DLinkedNode* node)
	{
		node->pre_->nxt_ = node->nxt_;
		node->nxt_->pre_ = node->pre_;
	}

	inline void erase(KeyType key)
	{
		auto it = kv_.find(key);
		assert(it != kv_.end());
		DLinkedNode* node = it->second;
		dlinkednode_remove(node);
		delete node;
		kv_.erase(it);
		--cacheCount_;
	}

	inline void insert(KeyType key, const std::shared_ptr<ValueType>& val)
	{
		assert(kv_.find(key) == kv_.end());
		DLinkedNode* node = new DLinkedNode(key, val);
		kv_.insert(std::make_pair(key, node));
		dlinkednode_push_back(node);
		if (++cacheCount_ > maxCacheCount_)
		{
			erase(head_.nxt_->key_);
		}
	}

	inline std::shared_ptr<ValueType> get(KeyType key)
	{
		++cacheGetCount_;
		auto it = kv_.find(key);
		if (it == kv_.end())
			return NULL;
		++cacheHitCount_;
		DLinkedNode* node = it->second;
		dlinkednode_remove(node);
		dlinkednode_push_back(node);
		return node->val_;
	}
public:
	LRUCache(size_t cacheBytes, bool islock)
		: cacheBytes_(cacheBytes)
		, perNodeBytes_(sizeof(KeyType) + sizeof(ValueType) + sizeof(std::shared_ptr<ValueType>))
		, cacheGetCount_(0)
		, cacheHitCount_(0)
		, cacheCount_(0)
		, islock_(islock)
	{
		maxCacheCount_ = cacheBytes_ / perNodeBytes_;
		if (maxCacheCount_ < kMinCacheCount_)
			maxCacheCount_ = kMinCacheCount_;
		if (maxCacheCount_ > kMaxCacheCount_)
			maxCacheCount_ = kMaxCacheCount_;
		head_.pre_ = head_.nxt_ = &tail_;
		tail_.pre_ = tail_.nxt_ = &head_;
	}

	~LRUCache()
	{
		DLinkedNode* node = head_.nxt_;
		while (node != &tail_)
		{
			DLinkedNode* tmp = node->nxt_;
			delete node;
			--cacheCount_;
			node = tmp;
		}
		assert(cacheCount_ == 0);
		head_.pre_ = head_.nxt_ = &tail_;
		tail_.pre_ = tail_.nxt_ = &head_;
		kv_.clear();
	}

	inline void Insert(KeyType key, const std::shared_ptr<ValueType>& val)
	{
		if (islock_) {
			Lock lock(&mutex_);
			insert(key, val);
		}
		else {
			insert(key, val);
		}
	}

	inline const std::shared_ptr<ValueType> Get(KeyType key)
	{
		if (islock_) {
			Lock lock(&mutex_);
			return get(key);
		}
		else {
			return get(key);
		}
	}

	inline void LogCacheHitRate()
	{
		fprintf(stdout, "cacheBytes_: %ld, perNodeBytes_: %ld, maxCacheCount_: %ld, cacheGetCount_: %ld, cacheHitCount_: %ld£¬ cacheHitRate: %.2f\n",
			cacheBytes_, perNodeBytes_, maxCacheCount_, cacheGetCount_, cacheHitCount_, cacheGetCount_ > 0 ? (double)cacheHitCount_ / cacheGetCount_ : 0);
	}
};

static void lock(std::mutex* m)
{
	m->lock();
}

static void unlock(std::mutex* m)
{
	m->unlock();
}

class Lock
{
public:
	explicit Lock(std::mutex* m)
		: m_mutexPtr(m, unlock) //Ö¸¶¨É¾³ýÆ÷
	{
		lock(m);
	}
private:
	std::shared_ptr<std::mutex> m_mutexPtr;
};

#endif /*LRU_CACHE_H_*/