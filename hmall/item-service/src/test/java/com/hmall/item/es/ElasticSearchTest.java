package com.hmall.item.es;

import cn.hutool.json.JSONUtil;
import com.hmall.item.domain.po.ItemDoc;
import org.apache.http.HttpHost;
import org.elasticsearch.action.search.SearchRequest;
import org.elasticsearch.action.search.SearchResponse;
import org.elasticsearch.client.RequestOptions;
import org.elasticsearch.client.RestClient;
import org.elasticsearch.client.RestHighLevelClient;
import org.elasticsearch.index.query.QueryBuilders;
import org.elasticsearch.search.SearchHit;
import org.elasticsearch.search.SearchHits;
import org.elasticsearch.search.aggregations.Aggregation;
import org.elasticsearch.search.aggregations.AggregationBuilders;
import org.elasticsearch.search.aggregations.Aggregations;
import org.elasticsearch.search.aggregations.bucket.terms.Terms;
import org.elasticsearch.search.builder.SearchSourceBuilder;
import org.elasticsearch.search.fetch.subphase.highlight.HighlightBuilder;
import org.elasticsearch.search.sort.SortOrder;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.boot.test.context.SpringBootTest;

import java.io.IOException;
import java.util.List;

public class ElasticSearchTest {

    private RestHighLevelClient client;

    @BeforeEach
    public void setUp() {
        client = new RestHighLevelClient(RestClient.builder(
                HttpHost.create("http://192.168.100.128:9200")
        ));
    }

    @AfterEach
    void tearDown() throws Exception {
        if (client != null) client.close();
    }

    @Test
    void testMatchAll() throws IOException {
        SearchRequest request = new SearchRequest("items");

        request.source()
                .query(QueryBuilders.boolQuery()
                        .must(QueryBuilders.matchQuery("name","牛奶"))
                        .filter(QueryBuilders.termQuery("brand","德"))
                        .filter(QueryBuilders.rangeQuery("price").lt(30000))
                );

        SearchResponse response = client.search(request, RequestOptions.DEFAULT);

        parseResponse(response);
    }

    @Test
    void testPageSearch() throws IOException {
        int pageNo = 1,pageSize=5;

        SearchRequest request = new SearchRequest("items");
        request.source()
                .query(QueryBuilders.matchQuery("name","牛奶"))
                .from((pageNo-1) * pageSize)
                .size(pageSize)
                .sort("sold", SortOrder.DESC)
                .sort("price", SortOrder.ASC);
        SearchResponse response = client.search(request, RequestOptions.DEFAULT);
        parseResponse(response);
    }

    @Test
    void testHighlight() throws IOException {
        SearchRequest request = new SearchRequest("items");

        request.source()
                .query(QueryBuilders.matchQuery("name","智能手机"))
                .highlighter(SearchSourceBuilder.highlight()
                        .field("name"));
        SearchResponse response = client.search(request, RequestOptions.DEFAULT);
        parseResponse(response);
    }

    @Test
    void testAgg() throws IOException{
        SearchRequest request = new SearchRequest("items");
        String brandAggName = "brandAgg";
        request.source()
                .size(0)
                .aggregation(
                        AggregationBuilders.terms(brandAggName)
                                .field("brand.keyword")
                                .size(10)
                );
        SearchResponse response = client.search(request, RequestOptions.DEFAULT);

        Aggregations aggregations = response.getAggregations();
        Terms terms = aggregations.get(brandAggName);
        List<? extends Terms.Bucket> buckets = terms.getBuckets();
        for (Terms.Bucket bucket : buckets) {
            System.out.println(bucket.getKeyAsString()+"==="+bucket.getDocCount());
        }
    }


    private static void parseResponse(SearchResponse response) {
        SearchHits hits = response.getHits();
        System.out.println("总条数"+hits.getTotalHits().value);
        SearchHit[] hitsHits = hits.getHits();
        for (SearchHit hit : hitsHits) {
            String json = hit.getSourceAsString();
            ItemDoc itemDoc = JSONUtil.toBean(json, ItemDoc.class);
            System.out.println(itemDoc);
        }
    }

}
