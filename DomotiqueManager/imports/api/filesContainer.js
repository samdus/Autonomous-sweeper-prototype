import { Mongo } from 'meteor/mongo';
 
export const FilesContainer = new FileCollection('FilesContainer',
    { resumable: true,    // Enable built-in resumable.js chunked upload support
        http: [             // Define HTTP route
        { method: 'get',  // Enable a GET endpoint
            path: '/:md5',  // this will be at route "/gridfs/myFiles/:md5"
            lookup: function (params, query) {  // uses express style url params
            return { md5: params.md5 };       // a query mapping url to myFiles
}}]});

if (Meteor.isServer) {
    Meteor.publish('FilesContainer',
    function () {
        console.log('publising files')
        return FilesContainer.find({ 'metadata._Resumable': { $exists: false }});
    });
}

if (Meteor.isServer) {
  Meteor.publish('FilesContainer',
    function () {
        return FilesContainer.find({ 'metadata._Resumable': { $exists: false }});
    });

    FilesContainer.allow({
    insert: function () {
      return true;
    },
    remove: function (userId, file) {
      return true;
    },
    read: function (userId, file) {
      return true;
    },
    write: function (userId, file, fields) {
      return true;
    }
  });
}